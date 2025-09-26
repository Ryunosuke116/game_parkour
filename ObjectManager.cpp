#include "common.h"
#include <vector>
#include <memory>
#include "ObjectManager.h"
#include "DebugDrawer.h"
#include "CollisionObjectManager.h"
#include "SubSystemManager.h"
#include "BlackOut.h"
#include "WorldSubSystem.h"
#include "boundaryRange.h"

/// <summary>
/// コンストラクタ
/// </summary>
ObjectManager::ObjectManager():
	streamStartPictureTimer(-1),
	stream_finishPicture_timer(-1),
	isStreamStartPicture(false),
	isStreamFinishPicture(false)
{

}

/// <summary>
/// デストラクタ	
/// </summary>
ObjectManager::~ObjectManager()
{
	managers.clear();
	objects.clear();
}

/// <summary>
/// オブジェクト生成
/// </summary>
void ObjectManager::Create()
{
	//vector型.atを使うとき用
	const int playerManagerNumber = 1;
	const int coinManagerNumber = 2;
	const int uiManagerNumber = 3;
	const int cameraObjectNumber = 2;
	const int shadowObjectNumber = 3;

	//生成
	//object生成
	objects.push_back(std::make_shared<SkyBox>());
	objects.push_back(std::make_shared<Field>());
	objects.push_back(std::make_shared<Camera>());
	objects.push_back(std::make_shared<Shadow>());

	layout				= std::make_shared<Layout>();
	tutorial			= std::make_shared<Tutorial>();
	finishCut			= std::make_shared<FinishCut>();
	L8TreeManager		= std::make_shared<Liner8TreeManager<CoinObject>>();

	//managerの生成
	managers.push_back(std::make_shared<CollisionObjectManager>());
	managers.push_back(std::make_shared<PlayerManager>());
	managers.push_back(std::make_shared<CoinManager>());
	managers.push_back(std::make_shared<UIManager>());
	
	//アップキャスト
	playerManager_actual	= std::dynamic_pointer_cast<PlayerManager>(managers.at(playerManagerNumber));
	coinManager_actual		= std::dynamic_pointer_cast<CoinManager>(managers.at(coinManagerNumber));
	shadow_actual			= std::dynamic_pointer_cast<Shadow>(objects.at(shadowObjectNumber));
	uiManager_actual		= std::dynamic_pointer_cast<UIManager>(managers.at(uiManagerNumber));

	//Jsonデータを取得
	for (auto& manager : managers)
	{
		manager->Create();
	}
	
	//コインオブザーバーに追加
	coinManager_actual->AddObserver(playerManager_actual->GetPlayer());
	coinManager_actual->AddObserver(uiManager_actual->GetUI_coin());

	//プレイヤーステートオブサーバーに追加
	playerManager_actual->AddObserver(uiManager_actual->GetUI_controlManual());

	camera_actual = std::dynamic_pointer_cast<Camera>(objects.at(cameraObjectNumber));

	//ロード
	for (auto& object : objects)
	{
		object->Create();
	}
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

	for (auto& manager : managers)
	{
		manager->Initialize();
	}

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
	stream_finishPicture_timer = 0.0f;
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

			for (auto& manager : managers)
			{
				manager->Update();
			}

		}
		else
		{
			coinManager_actual->Update();
			camera_actual->Update_layout();
			layout->Update(WorldSubSystem::GetInstance().GetSubSystem<Camera>()->GetScreenCenterPosition(),
				*coinManager_actual);
		}
		
		//ゴール判定
		if (uiManager_actual->GetGameTimer()->IsFinish() &&
			!isStreamFinishPicture)
		{
			isStreamFinishPicture = true;
			finishCut->SetIsDrawFinish(true);
		}
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
			const auto soundPlayer = SubSystemManager::GetInstance().GetSubSystem<SoundPlayer>().lock();
			uiManager_actual->GetGameTimer()->ResetSetTime();
			soundPlayer->Play("gameBGM");
		}
	}
	else
	{
		streamStartPictureTimer++;
		playerManager_actual->Update_start(streamStartPictureTimer);
		camera_actual->Update();
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

			BlackOut::GetInstance().GetAlpha() >= maxAlpha ? isGoal = true : isGoal = false;
		}

		playerManager_actual->Update_finish(streamStartPictureTimer);
		camera_actual->Update();
		if (isGoal)
		{
			auto soundPlayer = SubSystemManager::GetInstance().GetSubSystem<SoundPlayer>().lock();
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
	ShadowMap_DrawSetup(shadow_actual->GetShadowMapHandle());

	for (auto& manager : managers)
	{
		if (std::dynamic_pointer_cast<PlayerManager>(manager) ||
			std::dynamic_pointer_cast<CoinManager>(manager))
		{
			manager->Draw();
		}
	}

	//シャドウマップへの描画を終了
	ShadowMap_DrawEnd();

	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, shadow_actual->GetShadowMapHandle());

	for (auto& object : objects)
	{
		object->Draw();
	}

	for (auto& manager : managers)
	{
		if (auto uiManager = std::dynamic_pointer_cast<UIManager>(manager) &&
			isStreamStartPicture)
		{
			continue;
		}
		manager->Draw();
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

	//DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(10.0f, 15.0f, 0.0f), GetColor(255, 0, 0));
	//DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(0.0f, 25.0f, 0.0f), GetColor(0, 255, 0));
	//DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(0.0f, 15.0f, 10.0f), GetColor(0, 0, 255));
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
	const int coinManagerNumber = 3;
	const int shadowObjectNumber = 3;

	//生成
	//object生成
	objects.push_back(std::make_shared<SkyBox>());
	objects.push_back(std::make_shared<Field>());
	objects.push_back(std::make_shared<Camera>());
	objects.push_back(std::make_shared<Shadow>());

	//managerの生成
	managers.push_back(std::make_shared<CollisionObjectManager>());
	managers.push_back(std::make_shared<PlayerManager>());
	managers.push_back(std::make_shared<CoinManager>());
	managers.push_back(std::make_shared<UIManager>());

	//アップキャスト
	shadow_actual = std::dynamic_pointer_cast<Shadow>(objects.at(shadowObjectNumber));

	//Jsonデータを取得
	for (auto& manager : managers)
	{
		if (auto coinManager = std::dynamic_pointer_cast<CoinManager>(manager))
		{
			coinManager->ResultCreate(coinCount);
			continue;
		}

		if (auto uiManager = std::dynamic_pointer_cast<UIManager>(manager))
		{
			uiManager->ResultCreate(coinCount);
			continue;
		}
		manager->ResultCreate();
	}

	//ロード
	for (auto& object : objects)
	{
		object->ResultCreate();
	}
}

void ObjectManager::ResultInitilize()
{
	for (auto& manager : managers)
	{
		manager->ResultInitialize();
	}

	for (auto& object : objects)
	{
		object->ResultInitialize();
	}
}

void ObjectManager::ResultUpdate()
{
	for (auto& manager : managers)
	{
		manager->ResultUpdate();
	}

	for (auto& object : objects)
	{
		object->ResultUpdate();
	}
}

void ObjectManager::ResultDraw()
{
	// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(shadow_actual->GetShadowMapHandle());

	for (auto& manager : managers)
	{
		if (std::dynamic_pointer_cast<PlayerManager>(manager) ||
			std::dynamic_pointer_cast<CoinManager>(manager))
		{
			manager->Draw();
		}
	}

	//シャドウマップへの描画を終了
	ShadowMap_DrawEnd();

	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, shadow_actual->GetShadowMapHandle());

	for (auto& object : objects)
	{
		object->Draw();
	}

	for (auto& manager : managers)
	{
		if (auto uiManager = std::dynamic_pointer_cast<UIManager>(manager) &&
			isStreamStartPicture)
		{
			continue;
		}
		manager->Draw();
	}

	// 描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);
}