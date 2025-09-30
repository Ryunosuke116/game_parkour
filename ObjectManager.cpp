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

/// <summary>
/// コンストラクタ
/// </summary>
ObjectManager::ObjectManager() :
	streamStartPictureTimer(-1),
	streamFinishPictureTimer(-1),
	isStreamStartPicture(false),
	isStreamFinishPicture(false)
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
	const int cameraObjectNumber = 2;
	const int shadowObjectNumber = 3;
	const int coinManagerNumber = 5;
	const int playerManagerNumber = 6;
	const int uiManagerNumber = 7;

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
	actualCamera = std::dynamic_pointer_cast<Camera>(objects.at(cameraObjectNumber));
	actualShadow = std::dynamic_pointer_cast<Shadow>(objects.at(shadowObjectNumber));
	actualPlayerManager = std::dynamic_pointer_cast<PlayerManager>(objects.at(playerManagerNumber));
	actualCoinManager = std::dynamic_pointer_cast<CoinManager>(objects.at(coinManagerNumber));
	actualUiManager = std::dynamic_pointer_cast<UIManager>(objects.at(uiManagerNumber));

	//Jsonデータを取得
	for (auto& object : objects)
	{
		object->Create();
	}

	//コインオブザーバーに追加
	actualCoinManager->AddObserver(actualPlayerManager->GetPlayer());
	actualCoinManager->AddObserver(actualUiManager->GetUI_coin());

	//プレイヤーステートオブサーバーに追加
	actualPlayerManager->AddObserver(actualUiManager->GetUI_controlManual());

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

	FinishUpdate();

	if (!isStreamStartPicture &&
		!isStreamFinishPicture)
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
			actualCoinManager->Update();
			actualCamera->Update_layout();
			layout->Update(WorldSubSystem::GetInstance().GetSubSystem<Camera>()->GetScreenCenterPosition(),
				*actualCoinManager);
		}

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

	//ゴール判定
	if (actualUiManager->GetGameTimer()->IsFinish() &&
		!isStreamFinishPicture)
	{
		isStreamFinishPicture = true;
		finishCut->SetIsDrawFinish(true);
	}
}

void ObjectManager::StartUpdate()
{
	const int minAlpha = 0;
	const int addAlpha = 5;
	const float maxTimer = 50.0f;

	if (BlackOut::GetInstance().GetIsLightChange())
	{
		BlackOut::GetInstance().LightChangeUpdate(addAlpha);

		BlackOut::GetInstance().GetAlpha() <= minAlpha ?
			BlackOut::GetInstance().SetIsLightChange(false) :
			BlackOut::GetInstance().SetIsLightChange(true);
	}

	if (streamStartPictureTimer >= maxTimer)
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
		actualPlayerManager->Update_start(streamStartPictureTimer);
		actualCamera->Update();
	}
}

/// @brief 終了演出
void ObjectManager::FinishUpdate()
{
	if (isStreamFinishPicture)
	{
		BlackOut::GetInstance().SetIsLightChange(finishCut->Update());

		if (BlackOut::GetInstance().GetIsLightChange())
		{
			const int maxAlpha = 300;
			const int addAlpha = 5;
			BlackOut::GetInstance().BlackOutUpdate(addAlpha);

			BlackOut::GetInstance().GetAlpha() >= maxAlpha ? 
				isGoal = true :
				isGoal = false;
		}

		actualPlayerManager->Update_finish(streamStartPictureTimer);
		actualCamera->Update();
		if (isGoal)
		{
			auto soundPlayer = GameInstanceSubSystem::GetInstance().GetSubSystem<SoundPlayer>().lock();
			soundPlayer->Stop("gameBGM");
		}
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

void ObjectManager::ResultCreate(int coinCount)
{
	const int shadowObjectNumber = 3;

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
	actualShadow = std::dynamic_pointer_cast<Shadow>(objects.at(shadowObjectNumber));

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
		const int minAlpha = 0;
		const int addAlpha = 5;
		BlackOut::GetInstance().LightChangeUpdate(addAlpha);

		BlackOut::GetInstance().GetAlpha() <= minAlpha ?
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