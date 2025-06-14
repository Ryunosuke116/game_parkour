#include "BaseManager.h"
#include "FieldMesh.h"
#include "Include.h"

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
	map = std::make_shared<Map>("material/skyDome/sunSet.mv1");
	field = std::make_shared<Field>("material/mv1/new_city/0604.mv1");
	fieldMesh = std::make_shared<FieldMesh>("material/mv1/new_city/0610_mesh.mv1");
	coin = std::make_shared<CoinObject>("material/mv1/Coin.mv1");
	player = std::make_shared<Player>();
	camera = std::make_shared<Camera>();
	//playerManager = std::make_shared<PlayerManager>();
	actualPlayer = std::dynamic_pointer_cast<Player>(player);
	actualCoin = std::dynamic_pointer_cast<CoinObject>(coin);
}

/// <summary>
/// 初期化
/// </summary>
void ObjectManager::Initialize()
{
	map->Initialize();
	field->Initialize();
	fieldMesh->Initialize();
	coin->Initialize();
	player->Initialize();
	camera->Initialize();
}

/// <summary>
/// 更新
/// </summary>
void ObjectManager::Update()
{
	actualPlayer->Update(camera->GetCameraDirection(),fieldMesh->GetModelHandle());
	camera->Update(player->GetPosition());
	//playerManager->Update(fieldMesh->GetModelHandle(), *actualPlayer);
	actualCoin->Update(actualPlayer->GetTopPos(), actualPlayer->GetBottomPos(), actualPlayer->GetRadius());
	map->Update();
	field->Update();
	fieldMesh->Update();
}

/// <summary>
/// 描画
/// </summary>
void ObjectManager::Draw()
{
	player->Draw();
	camera->Draw();
	//playerManager->Draw();
	map->Draw();
	field->Draw();
	coin->Draw();
	fieldMesh->Draw();
}