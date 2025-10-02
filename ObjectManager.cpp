#include "common.h"
#include <vector>
#include <memory>
#include "ObjectManager.h"
#include "DebugDrawer.h"
#include "CollisionObjectManager.h"
#include "GameInstanceSubSystem.h"
#include "BlackOut.h"
#include "WorldSubSystem.h"
#include "boundaryRange.h"
#include "BackGround.h"

/// <summary>
/// コンストラクタ
/// </summary>
ObjectManager::ObjectManager() :
	streamStartPictureTimer(-1.0f),
	streamFinishPictureTimer(-1.0f),
	isStreamStartPicture(false),
	isStreamFinishPicture(false),
	isGoal(false),
	isPush(false),
	isCamera(false)
{

}

/// <summary>
/// デストラクタ	
/// </summary>
ObjectManager::~ObjectManager()
{
	objects.clear();
}

/// <summary>
/// オブジェクト生成
/// </summary>
void ObjectManager::Create()
{
	//vector型.atを使うとき用
	const int kCameraObjectNumber = 2;
	const int kShadowObjectNumber = 3;
	const int kCoinManagerNumber = 5;
	const int kPlayerManagerNumber = 6;
	const int kUiManagerNumber = 7;

	//生成
	//object生成
	objects.push_back(std::make_shared<SkyBox>());
	objects.push_back(std::make_shared<Field>());
	objects.push_back(std::make_shared<Camera>());
	objects.push_back(std::make_shared<Shadow>());

	layout = std::make_shared<Layout>();
	tutorial = std::make_shared<Tutorial>();
	finishCut = std::make_shared<FinishCut>();
	L8TreeManager = std::make_shared<Liner8TreeManager<Coin>>();

	//managerの生成
	objects.push_back(std::make_shared<CollisionObjectManager>());
	objects.push_back(std::make_shared<CoinManager>());
	objects.push_back(std::make_shared<PlayerManager>());
	objects.push_back(std::make_shared<UIManager>());

	//アップキャスト
	actualCamera = std::dynamic_pointer_cast<Camera>(objects.at(kCameraObjectNumber));
	actualShadow = std::dynamic_pointer_cast<Shadow>(objects.at(kShadowObjectNumber));
	actualPlayerManager = std::dynamic_pointer_cast<PlayerManager>(objects.at(kPlayerManagerNumber));
	actualCoinManager = std::dynamic_pointer_cast<CoinManager>(objects.at(kCoinManagerNumber));
	actualUiManager = std::dynamic_pointer_cast<UIManager>(objects.at(kUiManagerNumber));

	//Jsonデータを取得
	for (auto& object : objects)
	{
		object->Create();
	}

	//コインオブザーバーに追加
	actualCoinManager->AddObserver(actualPlayerManager->GetPlayer());
	actualCoinManager->AddObserver(actualUiManager->GetCoinUi());

	//プレイヤーステートオブサーバーに追加
	actualPlayerManager->AddObserver(actualUiManager->GetControlManualUi());

	//ロード
	L8TreeManager->Create();

	tutorial->Load(JsonManager::GetInstance().GetJsons(tutorial->GetTag()));
	finishCut->Load(JsonManager::GetInstance().GetJsons(finishCut->GetTag()));
}

/// <summary>
/// 初期化
/// </summary>
void ObjectManager::Initialize()
{
	const int spaceLevel = 6;
	L8TreeManager->Initialize(spaceLevel, BoundaryRange::min, BoundaryRange::max);

	for (auto& object : objects)
	{
		object->Initialize();
	}

	tutorial->Initialize();
	finishCut->Initialize();

	isCamera = false;
	isPush = false;
	isGoal = false;
	isStreamStartPicture = true;
	isStreamFinishPicture = false;
	streamStartPictureTimer = 0.0f;
	streamFinishPictureTimer = 0.0f;
}

/// <summary>
/// 更新
/// </summary>
void ObjectManager::Update()
{
	if (isStreamStartPicture)
	{
		StartUpdate();
	}

	if (isStreamFinishPicture)
	{
		FinishUpdate();
	}

	if (!isStreamStartPicture &&
		!isStreamFinishPicture)
	{
		// Release用のUpdate
		#if defined(NDEBUG)
			NormalUpdate();
		#else
		//　Debug用のUpdate
			WhenDebugUpdate();
		#endif

		//ゴール判定
		if (actualUiManager->GetGameTimer()->IsFinish() &&
			!isStreamFinishPicture)
		{
			isStreamFinishPicture = true;
			finishCut->SetIsDrawFinish(true);
			const auto soundPlayer = GameInstanceSubSystem::GetInstance().GetSubSystem<SoundPlayer>().lock();
			soundPlayer->Play("gool");
		}
	}
}

void ObjectManager::NormalUpdate()
{
	for (auto& object : objects)
	{
		object->Update();
	}
}

void ObjectManager::WhenDebugUpdate()
{
	if (CheckHitKey(KEY_INPUT_0))
	{
		if (!isPush)
		{
			if (!isCamera)
			{
				isCamera = true;
				layout->Initialize(WorldSubSystem::GetInstance().GetSubSystem<CoinManager>()->GetModelHandle());
			}
			else
			{
				isCamera = false;
			}

			isPush = true;
		}
	}
	else
	{
		isPush = false;
	}

	if (!isCamera)
	{
		for (auto& object : objects)
		{
			object->Update();
		}

	}
	else
	{
		actualShadow->Update();
		actualCoinManager->Update();
		actualCamera->LayOutUpdate();
		layout->Update(WorldSubSystem::GetInstance().GetSubSystem<Camera>()->GetScreenCenterPosition(),
			*actualCoinManager);
	}
}

void ObjectManager::StartUpdate()
{
	const int kMinAlpha = 0;
	const int kAddAlpha = 5;
	const float kMaxTimer = 50.0f;

	//画面の明転処理
	if (BlackOut::GetInstance().GetIsLightChange())
	{
		BlackOut::GetInstance().LightChangeUpdate(kAddAlpha);

		BlackOut::GetInstance().GetAlpha() <= kMinAlpha ?
			BlackOut::GetInstance().SetIsLightChange(false) :
			BlackOut::GetInstance().SetIsLightChange(true);
	}

	if (streamStartPictureTimer >= kMaxTimer)
	{
		isStreamStartPicture = tutorial->Update();
		streamStartPictureTimer++;

		if (!isStreamStartPicture)
		{
			const auto soundPlayer = GameInstanceSubSystem::GetInstance().GetSubSystem<SoundPlayer>().lock();
			actualUiManager->GetGameTimer()->ResetSetTime();
			soundPlayer->Play("gameBGM");
		}
	}
	else
	{
		streamStartPictureTimer++;
		actualShadow->Update();
		actualPlayerManager->StartUpdate(streamStartPictureTimer);
		actualCamera->Update();
	}
}

/// @brief 終了演出
void ObjectManager::FinishUpdate()
{
	BlackOut::GetInstance().SetIsLightChange(finishCut->Update());

	if (BlackOut::GetInstance().GetIsLightChange())
	{
		const int kMaxAlpha = 300;
		const int kAddAlpha = 5;
		BlackOut::GetInstance().BlackOutUpdate(kAddAlpha);

		BlackOut::GetInstance().GetAlpha() >= kMaxAlpha ?
			isGoal = true :
			isGoal = false;
	}

	actualPlayerManager->FinishUpdate(streamStartPictureTimer);
	actualCamera->Update();
	if (isGoal)
	{
		auto soundPlayer = GameInstanceSubSystem::GetInstance().GetSubSystem<SoundPlayer>().lock();
		soundPlayer->Stop("gameBGM");
	}
}

/// <summary>
/// 描画
/// </summary>
void ObjectManager::Draw()
{
	// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(actualShadow->GetShadowMapHandle());

	for (auto& object : objects)
	{
		if (std::dynamic_pointer_cast<PlayerManager>(object) ||
			std::dynamic_pointer_cast<CoinManager>(object))
		{
			object->Draw();
		}
	}

	//シャドウマップへの描画を終了
	ShadowMap_DrawEnd();

	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, actualShadow->GetShadowMapHandle());

	for (auto& object : objects)
	{
		if (std::dynamic_pointer_cast<UIManager>(object) &&
			isStreamStartPicture)
		{
			continue;
		}
		object->Draw();
	}

	// 描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);

	if (isCamera)
	{
		layout->Draw();
	}

	tutorialDraw();

	finishCut->Draw();

	BlackOut::GetInstance().Draw();
}

void ObjectManager::tutorialDraw()
{
	if (isStreamStartPicture)
	{
		tutorial->Draw(streamStartPictureTimer);
	}
}

void ObjectManager::TitleCreate()
{
	//-----------------------------------------//
	// 2D描画するobjectを追加する場合
	// 先に描画したい順にリストに追加する
	//----------------------------------------//

	objects.push_back(std::make_shared<BackGround>());

	for (auto& object : objects)
	{
		object->Create();
	}
}

void ObjectManager::TitleInitilize()
{
	for (auto& object : objects)
	{
		object->Initialize();
	}
}

void ObjectManager::TitleUpdate()
{
	for (auto& object : objects)
	{
		object->Update();
	}
}

void ObjectManager::TitleDraw()
{
	for (auto& object : objects)
	{
		object->Draw();
	}
}

void ObjectManager::ResultCreate(int coinCount)
{
	const int kShadowObjectNumber = 3;

	//生成
	//object生成
	objects.push_back(std::make_shared<SkyBox>());
	objects.push_back(std::make_shared<Field>());
	objects.push_back(std::make_shared<Camera>());
	objects.push_back(std::make_shared<Shadow>());

	//managerの生成
	objects.push_back(std::make_shared<CollisionObjectManager>());
	objects.push_back(std::make_shared<PlayerManager>());
	objects.push_back(std::make_shared<UIManager>());

	//アップキャスト
	actualShadow = std::dynamic_pointer_cast<Shadow>(objects.at(kShadowObjectNumber));

	//ロード
	for (auto& object : objects)
	{
		if (auto uiManager = std::dynamic_pointer_cast<UIManager>(object))
		{
			uiManager->ResultCreate(coinCount);
			continue;
		}

		object->ResultCreate();
	}
}

void ObjectManager::ResultInitilize()
{
	for (auto& object : objects)
	{
		object->ResultInitialize();
	}
}

void ObjectManager::ResultUpdate()
{
 	if (BlackOut::GetInstance().GetIsLightChange())
	{
		const int kMinAlpha = 0;
		const int kAddAlpha = 5;
		BlackOut::GetInstance().LightChangeUpdate(kAddAlpha);

		BlackOut::GetInstance().GetAlpha() <= kMinAlpha ?
			BlackOut::GetInstance().SetIsLightChange(false) :
			BlackOut::GetInstance().SetIsLightChange(true);
	}

	for (auto& object : objects)
	{
		object->ResultUpdate();
	}
}

void ObjectManager::ResultDraw()
{
	// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(actualShadow->GetShadowMapHandle());

	for (auto& object : objects)
	{
		if (std::dynamic_pointer_cast<PlayerManager>(object))
		{
			object->Draw();
		}
	}

	//シャドウマップへの描画を終了
	ShadowMap_DrawEnd();

	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, actualShadow->GetShadowMapHandle());

	for (auto& object : objects)
	{
		if (std::dynamic_pointer_cast<UIManager>(object) &&
			isStreamStartPicture)
		{
			continue;
		}
		object->Draw();
	}

	// 描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);
}