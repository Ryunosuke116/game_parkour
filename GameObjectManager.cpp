#include "common.h"
#include <vector>
#include <memory>
#include "GameObjectManager.h"

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


	map				= std::make_shared<Map>("material/skyDome/sunSet.mv1");
	field			= std::make_shared<Field>("material/mv1/new_city/new_city_0731.mv1");
	coinManager		= std::make_shared<CoinManager>();
	playerManager	= std::make_shared<PlayerManager>();
	camera			= std::make_shared<Camera>();
	layout			= std::make_shared<Layout>();
	shadow			= std::make_shared<Shadow>();
	ui				= std::make_shared<UI>();
	
	map_actual			 = std::dynamic_pointer_cast<Map>(map);
	playerManager_actual = std::dynamic_pointer_cast<PlayerManager>(playerManager);
	coinManager_actual   = std::dynamic_pointer_cast<CoinManager>(coinManager);


	fieldObjects.push_back(std::make_shared<FieldMesh>("material/mv1/new_city/new_city_mesh_0731.mv1"));
	fieldObjects.push_back(std::make_shared<Floor_sky>(jsonManager->GetJsons("object")));
	fieldObjects.push_back(std::make_shared<Wall>(jsonManager->GetJsons("object")));

	coinManager->HandOver(jsonManager->GetJsons("coin"));
	playerManager->HandOver(jsonManager->GetJsons("player"));

	playerManager->Create();
	coinManager->Create();
	coinManager_actual->AddObserver(playerManager_actual->GetPlayer());
	coinManager_actual->AddObserver(ui);
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

	map->Initialize();
	field->Initialize();
	coinManager->Initialize();
	playerManager->Initialize();
	camera->Initialize();
	PadInput::Initialize();
	ui->Initialize();
	shadow->Initialize();

	isCamera = false;
	isPush = false;

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
	
		playerManager_actual->Update(fieldObjects, camera->GetCameraDirection());
		map_actual->Update(playerManager_actual->GetPosition());
		camera->Update(playerManager_actual->GetPosition(),fieldObjects);

		ui->Update();
	}
	else
	{
		camera->Update_layout();
		layout->Update(camera->GetSpherePosition(), *coinManager_actual);
	}

	coinManager_actual->Update(playerManager_actual->GetPlayer(),camera->GetSpherePosition());

}

/// <summary>
/// 描画
/// </summary>
bool GameObjectManager::Draw()
{
	// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(shadow->GetShadowMapHandle());

	//field->Draw();
	/*for (auto& fieldObject : fieldObjects)
	{
		if ("field" != fieldObject->GetTag())
		{
			fieldObject->Draw();
		}
	}*/
	playerManager->Draw();
	coinManager->Draw();

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
	playerManager->Draw();
	coinManager->Draw();


	// 描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);

	camera->Draw();
	if (isCamera)
	{
		layout->Draw();
	}
	ui->Draw();

	DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(10.0f, 15.0f, 0.0f), GetColor(255, 0, 0));
	DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(0.0f, 25.0f, 0.0f), GetColor(0, 255, 0));
	DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(0.0f, 15.0f, 10.0f), GetColor(0, 0, 255));
	return true;
}
