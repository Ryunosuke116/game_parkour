#include "common.h"
#include <vector>
#include <memory>
#include "GameObjectManager.h"
#include "DebugDrawer.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameObjectManager::GameObjectManager()
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
	jsonManager		= std::make_shared<JsonManager>();
	jsonManager->Initialize();
	

	managers.push_back(std::make_shared<PlayerManager>());
	playerManager_actual = std::dynamic_pointer_cast<PlayerManager>(managers.back());
	managers.push_back(std::make_shared<UIManager>());
	uiManager_actual = std::dynamic_pointer_cast<UIManager>(managers.back());
	managers.push_back(std::make_shared<CoinManager>());
	coinManager_actual = std::dynamic_pointer_cast<CoinManager>(managers.back());
	


	//生成
	map					= std::make_shared<Map>("material/skyDome/sunSet.mv1");
	field				= std::make_shared<Field>();

	camera				= std::make_shared<Camera>();
	layout				= std::make_shared<Layout>();
	shadow				= std::make_shared<Shadow>();
	goalArea			= std::make_shared<GoalArea>();
	gameTimer			= std::make_shared<GameTimer>();

	map_actual			 = std::dynamic_pointer_cast<Map>(map);

	fieldObjects.push_back(std::make_shared<FieldMesh>("material/mv1/new_city/new_city_mesh_0731.mv1"));

	
	//floor_skyを追加
	nlohmann::json data_floor_sky = jsonManager->GetJsons("floor_sky");
	std::string path_floor_sky = data_floor_sky["modelPath"];
	for (auto& data : data_floor_sky["list"])
	{
		int modelHandle = MV1LoadModel(path_floor_sky.c_str());
		float degree = data[3];
		std::string tag = data[4].get<std::string>();

		fieldObjects.push_back(std::make_shared<Floor_sky>(
			modelHandle,
			VGet(data[0], data[1], data[2]),
			degree,
			tag
		));
	}

	//Jsonデータを取得
	for (auto& manager : managers)
	{
		manager->HandOver(jsonManager->GetJsons(manager->GetTag()));
		manager->Create();
	}

	managers.push_back(std::make_shared<EffectManager>());
	effectManager_actual = std::dynamic_pointer_cast<EffectManager>(managers.back());

	//コインオブザーバーを追加
	coinManager_actual->AddObserver(playerManager_actual->GetPlayer());
	coinManager_actual->AddObserver(uiManager_actual->GetUI_coin());

	//ロード
	field->Load(jsonManager->GetJsons("field"));


}

/// <summary>
/// 初期化
/// </summary>
void GameObjectManager::Initialize()
{

	for (auto& fieldObject : fieldObjects)
	{
		fieldObject->Initialize();
	}

	for (auto& manager : managers)
	{
		manager->Initialize();
	}

	map->Initialize();
	field->Initialize();
	camera->Initialize();
	PadInput::Initialize();
	shadow->Initialize();
	goalArea->Initialize();
	gameTimer->Initialize();

	//エフェクトデータを追加
	//TODO::やり方が悪いので修正しなければ
	nlohmann::json effectData = jsonManager->GetJsons("effectData");
	for (auto& data : effectData["list"])
	{
		std::string tag = data[1];
		std::string  path = data[0].get<std::string>();
		float scale = data[2];

		effectManager_actual->Add(path.c_str(), tag, scale);
	}

	isCamera = false;
	isPush = false;
	isGoal = false;
}

/// <summary>
/// 更新
/// </summary>
void GameObjectManager::Update()
{

	shadow->Update(playerManager_actual->GetPosition());

	if (CheckHitKey(KEY_INPUT_0))
	{
		if (!isPush)
		{
			if (!isCamera)
			{
				isCamera = true;
				layout->Initialize(coinManager_actual->GetModelHandle());
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
		for (auto& fieldObject : fieldObjects)
		{
			fieldObject->Update();
		}
		field->Update();

		gameTimer->Update();
		playerManager_actual->Update(fieldObjects,effectManager_actual, camera->GetCameraDirection());
		map_actual->Update(playerManager_actual->GetPosition());
		camera->Update(playerManager_actual->GetPosition(),
			playerManager_actual->GetAngle(), fieldObjects);

		//ui->Update();
		uiManager_actual->Update();
	}
	else
	{
		camera->Update_layout();
		layout->Update(camera->GetSpherePosition(), *coinManager_actual);
	}

	coinManager_actual->Update(playerManager_actual->GetPlayer(),
		effectManager_actual,
		camera->GetSpherePosition());

	effectManager_actual->PlayEffectUpdate();

	if (HitCheck::AABBHitJudge(playerManager_actual->GetPlayerAABB(),
		goalArea->GetGoalArea()
	))
	{
		isGoal = true;
	}
	else
	{
		isGoal = false;
	}
	DebugDrawer::Instance().InformationInput_string_bool("isGoal %d\n", isGoal);
}

/// <summary>
/// 描画
/// </summary>
void GameObjectManager::Draw()
{
	// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(shadow->GetShadowMapHandle());

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
	SetUseShadowMap(0, shadow->GetShadowMapHandle());

	map->Draw();
	field->Draw();
	for (auto& fieldObject : fieldObjects)
	{
		fieldObject->Draw();
	}

	for (auto& manager : managers)
	{
		manager->Draw();
	}


	// 描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);

	camera->Draw();
	if (isCamera)
	{
		layout->Draw();
	}

	DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(10.0f, 15.0f, 0.0f), GetColor(255, 0, 0));
	DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(0.0f, 25.0f, 0.0f), GetColor(0, 255, 0));
	DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(0.0f, 15.0f, 10.0f), GetColor(0, 0, 255));
}
