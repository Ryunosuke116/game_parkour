#include "common.h"
#include "Player.h"
#include "CollisionManager.h"
#include "PlayerManager.h"
#include "Calculation.h"
#include "BaseObject.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerManager::PlayerManager()
{
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerManager::~PlayerManager()
{

}

void PlayerManager::Create() 
{
	collisionManager = std::make_shared<CollisionManager>();

	player = std::make_shared<Player>(jsonData);
	actualPlayer = std::dynamic_pointer_cast<Player>(player);
	//characters.push_back(std::make_shared<Player>(jsonData));
}

/// <summary>
/// 初期化
/// </summary>
void PlayerManager::Initialize()
{
	actualPlayer->Initialize();

	/*for (auto& chara : characters)
	{
		chara->Initialize();
	}*/
}

/// <summary>
/// 更新
/// </summary>
/// <param name="mapHandle"></param>
/// <param name="player"></param>
void PlayerManager::Update(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects ,
	const VECTOR& cameraDirection)
{

	actualPlayer->Update(cameraDirection, fieldObjects);

	collisionManager->Update(*player, fieldObjects, actualPlayer->GetData());
	
	actualPlayer->Receive_CollisionResult();

	actualPlayer->PositionUpdate();
	
}

/// <summary>
/// 描画
/// </summary>
void PlayerManager::Draw()
{
	actualPlayer->Draw();

	/*for (auto& chara : characters)
	{
		chara->Draw();
	}*/
	collisionManager->Draw();
	collisionManager->Draw();
}

VECTOR PlayerManager::PositionCheck(const VECTOR& hangingPos, const VECTOR& playerPos)
{
	VECTOR newPlayerPos = playerPos;
	VECTOR hanging = hangingPos;
	hanging.y = playerPos.y;

	Calculation::Leap(newPlayerPos, hanging, 0.1f);

	return newPlayerPos;
}

void PlayerManager::Update() {}
void PlayerManager::Add()	 {}