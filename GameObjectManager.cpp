#include "common.h"
#include <vector>
#include <memory>
#include "GameObjectManager.h"
#include "DebugDrawer.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameObjectManager::GameObjectManager():
	stream_startPicture_timer(-1),
	stream_finishPicture_timer(-1),
	isStream_startPicture(false),
	isStream_finishPicture(false)
{

}

/// <summary>
/// デストラクタ	
/// </summary>
GameObjectManager::~GameObjectManager()
{

}

/// <summary>
/// オブジェクト生成
/// </summary>
void GameObjectManager::Create()
{
	//vector型.atを使うとき用
	const int effectManagerNumber = 0;
	const int playerManagerNumber = 1;
	const int uiManagerNumber = 2;
	const int coinManagerNumber = 3;
	const int cameraObjectNumber = 2;
	const int shadowObjectNumber = 3;

	//生成
	//collisionObjectの生成
	collisionObjects.push_back(std::make_shared<FieldMesh>());
	collisionObjects.back()->Load(JsonManager::Instance().GetJsons(collisionObjects.back()->GetJsonTag()));
	
	//floor_skyを追加
	nlohmann::json data_floor_sky = JsonManager::Instance().GetJsons("floor_sky");
	std::string path_floor_sky = data_floor_sky["modelPath"];
	for (auto& data : data_floor_sky["list"])
	{
		int modelHandle = MV1LoadModel(path_floor_sky.c_str());
		float degree = data[3];
		std::string tag = data[4].get<std::string>();

		collisionObjects.push_back(std::make_shared<Floor_sky>(
			modelHandle,
			VGet(data[0], data[1], data[2]),
			degree,
			tag
		));
	}

	//object生成
	objects.push_back(std::make_shared<SkyBox>());
	objects.push_back(std::make_shared<Field>());
	objects.push_back(std::make_shared<Camera>());
	objects.push_back(std::make_shared<Shadow>());

	layout				= std::make_shared<Layout>();
	tutorial			= std::make_shared<Tutorial>();
	finishCut			= std::make_shared<FinishCut>();
	soundPlayer			= std::make_shared<SoundPlayer>();

	//managerの生成
	managers.push_back(std::make_shared<EffectManager>());
	managers.push_back(std::make_shared<PlayerManager>());
	managers.push_back(std::make_shared<UIManager>());
	managers.push_back(std::make_shared<CoinManager>());
	
	
	//アップキャスト
	std::shared_ptr<BaseGameObjectManager> soundPlayer_actual = std::dynamic_pointer_cast<SoundPlayer>(soundPlayer);
	effectManager			 = std::dynamic_pointer_cast<EffectManager>(managers.at(effectManagerNumber));
	playerManager_actual = std::dynamic_pointer_cast<PlayerManager>(managers.at(playerManagerNumber));
	uiManager_actual = std::dynamic_pointer_cast<UIManager>(managers.at(uiManagerNumber));
	coinManager_actual = std::dynamic_pointer_cast<CoinManager>(managers.at(coinManagerNumber));
	shadow_actual			= std::dynamic_pointer_cast<Shadow>(objects.at(shadowObjectNumber));

	//Jsonデータを取得
	for (auto& manager : managers)
	{
		manager->HandOver(JsonManager::Instance().GetJsons(manager->GetTag()));
		manager->Create();
	}
	soundPlayer_actual->HandOver(JsonManager::Instance().GetJsons(soundPlayer_actual->GetTag()));

	//コインオブザーバーに追加
	coinManager_actual->AddObserver(playerManager_actual->GetPlayer());
	coinManager_actual->AddObserver(uiManager_actual->GetUI_coin());

	//プレイヤーステートオブサーバーに追加
	playerManager_actual->AddObserver(uiManager_actual->GetUI_controlManual());

	camera_actual = std::dynamic_pointer_cast<Camera>(objects.at(cameraObjectNumber));

	objectMediator = std::make_shared<ObjectMediator>(
		*soundPlayer,
		*effectManager,
		*playerManager_actual->GetPlayer(),
		*camera_actual,
		collisionObjects);

	//ロード
	for (auto& object : objects)
	{
		if (object->GetJsonTag() == "")continue;
		object->Load(JsonManager::Instance().GetJsons(object->GetJsonTag()));
	}

	tutorial->Load(JsonManager::Instance().GetJsons(tutorial->GetTag()));
	finishCut->Load(JsonManager::Instance().GetJsons(finishCut->GetTag()));
	soundPlayer->Create();

	soundHandle = LoadSoundMem("material/sound/gameBGM.mp3");
	ChangeVolumeSoundMem(125, soundHandle);
}

/// <summary>
/// 初期化
/// </summary>
void GameObjectManager::Initialize()
{
	for (auto& fieldObject : collisionObjects)
	{
		fieldObject->Initialize();
	}

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
	isStream_startPicture = true;
	isStream_finishPicture = false;
	stream_startPicture_timer = 0.0f;
	stream_finishPicture_timer = 0.0f;
}

/// <summary>
/// 更新
/// </summary>
void GameObjectManager::Update()
{
	if (isStream_startPicture)
	{
		StartUpdate();
	}

	FinishUpdate();

	if (!isStream_startPicture && 
		!isStream_finishPicture)
	{
		if (CheckHitKey(KEY_INPUT_0))
		{
			if (!isPush)
			{
				if (!isCamera)
				{
					isCamera = true;
					//layout->Initialize(coinManager_actual->GetModelHandle());
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
			for (auto& collisionObject : collisionObjects)
			{
				collisionObject->Update(*objectMediator);
			}

			for (auto& object : objects)
			{
				object->Update(*objectMediator);
			}

			for (auto& manager : managers)
			{
				manager->Update(*objectMediator);
			}

		}
		else
		{
			camera_actual->Update_layout();
			//layout->Update(camera_actual->GetSpherePosition(), *coinManager_actual);
		}

		effectManager->PlayEffectUpdate();

		//ゴール判定
		if (uiManager_actual->GetGameTimer()->IsFinish() &&
			!isStream_finishPicture)
		{
			isStream_finishPicture = true;
			finishCut->SetIsDraw_finish(true);
		}
	}
}

void GameObjectManager::StartUpdate()
{
	if (stream_startPicture_timer >= 50.0f)
	{
		isStream_startPicture = tutorial->Update();
		stream_startPicture_timer++;

		if (!isStream_startPicture)
		{
			uiManager_actual->GetGameTimer()->ResetSetTime();
			soundPlayer->Play("gameBGM");
		}
	}
	else
	{
		stream_startPicture_timer++;
		playerManager_actual->Update_start(stream_startPicture_timer);
		camera_actual->Update(*objectMediator);
	}
}

void GameObjectManager::FinishUpdate()
{
	if (isStream_finishPicture)
	{
		isGoal = finishCut->Update();
		playerManager_actual->Update_finish(stream_startPicture_timer);
		camera_actual->Update(*objectMediator);
		if (isGoal)
		{
			StopSoundMem(soundHandle);
		}
	}
}

/// <summary>
/// 描画
/// </summary>
void GameObjectManager::Draw()
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

	for (auto& fieldObject : collisionObjects)
	{
		fieldObject->Draw();
	}

	for (auto& manager : managers)
	{
		if (auto uiManager = std::dynamic_pointer_cast<UIManager>(manager) &&
			isStream_startPicture)
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

	//DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(10.0f, 15.0f, 0.0f), GetColor(255, 0, 0));
	//DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(0.0f, 25.0f, 0.0f), GetColor(0, 255, 0));
	//DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(0.0f, 15.0f, 10.0f), GetColor(0, 0, 255));
}

void GameObjectManager::tutorialDraw()
{
	if (isStream_startPicture)
	{
		tutorial->Draw(stream_startPicture_timer);
	}
}