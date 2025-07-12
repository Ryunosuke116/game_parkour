#include "common.h"
#include "ObjectManager.h"
#include "Json.h"

/// <summary>
/// コンストラクタ
/// </summary>
ObjectManager::ObjectManager()
{
	
}

/// <summary>
/// デストラクタ	
/// </summary>
ObjectManager::~ObjectManager()
{

}

/// <summary>
/// オブジェクト生成
/// </summary>
void ObjectManager::Create()
{
	map				= std::make_shared<Map>("material/skyDome/sunSet.mv1");
	field			= std::make_shared<Field>("material/mv1/new_city/new_city_0710.mv1");
	fieldMesh		= std::make_shared<FieldMesh>("material/mv1/new_city/new_city_mesh_0710.mv1");
	coinManager		= std::make_shared<CoinManager>();
	playerManager	= std::make_shared<PlayerManager>();
	camera			= std::make_shared<Camera>();
	layout			= std::make_shared<Layout>();
	jsonManager = std::make_shared<JsonManager>();
	
	map_actual			 = std::dynamic_pointer_cast<Map>(map);
	playerManager_actual = std::dynamic_pointer_cast<PlayerManager>(playerManager);
	coinManager_actual   = std::dynamic_pointer_cast<CoinManager>(coinManager);

	
}

/// <summary>
/// 初期化
/// </summary>
void ObjectManager::Initialize()
{
	map->Initialize();
	field->Initialize();
	fieldMesh->Initialize();
	coinManager->Initialize();
	playerManager->Initialize();
	camera->Initialize();
	PadInput::Initialize();
	jsonManager->Initialize();

	isCamera = false;
	isPush = false;

}

/// <summary>
/// 更新
/// </summary>
void ObjectManager::Update()
{
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
		camera->Update(playerManager_actual->GetPlayer()->GetPosition(),fieldMesh->GetModelHandle());
	
		playerManager_actual->Update(fieldMesh->GetModelHandle(), camera->GetCameraDirection());
		map_actual->Update(playerManager_actual->GetPlayer()->GetPosition());
		field->Update();
		fieldMesh->Update();
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
bool ObjectManager::Draw()
{
	playerManager->Draw();
	camera->Draw();
	map->Draw();
	field->Draw();
	fieldMesh->Draw();
	coinManager->Draw();
	if (isCamera)
	{
		layout->Draw();
	}

	DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(10.0f, 15.0f, 0.0f), GetColor(255, 0, 0));
	DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(0.0f, 25.0f, 0.0f), GetColor(0, 255, 0));
	DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(0.0f, 15.0f, 10.0f), GetColor(0, 0, 255));
	return true;
}
