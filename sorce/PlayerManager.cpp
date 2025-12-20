#include "Common.h"
#include <vector>
#include "Player.h"
#include "CollisionManager.h"
#include "PlayerManager.h"
#include "Calculation.h"
#include "BaseObject.h"
#include "DebugDrawer.h"
#include "GameInstanceSubSystem.h"
#include "WorldSubSystem.h"
#include "PlayerCalculation.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerManager::PlayerManager() :
	BaseGameObjectManager(),
	nowPlayerData({ false })
{
	tag = "player";
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerManager::~PlayerManager()
{
	observers.clear();
}

void PlayerManager::Create()
{
	auto self = shared_from_this();

	collisionManager = std::make_shared<CollisionManager>();

	player = std::make_shared<Player>();
	actualPlayer = std::dynamic_pointer_cast<Player>(player);
	player->Create();

	//サブシステムに追加
	WorldSubSystem::GetInstance().AddSubSystem<PlayerManager>(self);
}

/// <summary>
/// 初期化
/// </summary>
void PlayerManager::Initialize()
{
	actualPlayer->Initialize();

	nowPlayerData = { false };
}

/// <summary>
/// 更新
/// </summary>
/// <param name="mapHandle"></param>
/// <param name="player"></param>
void PlayerManager::Update()
{
	actualPlayer->Update();

	if (actualPlayer->GetData().isRunWall)
	{
		VECTOR oldPos = player->GetPosition();
		VECTOR newPos = VAdd(oldPos, player->GetVelocity());

		VECTOR newPositon = collisionManager->WallGroundCollisionCheck(
			WorldSubSystem::GetInstance().GetSubSystem<CollisionObjectManager>()->GetCollisionObjects(),
			oldPos,
			newPos,
			player->GetVelocity(),
			actualPlayer->playerCalculation->GetWallRunGravity(),
			player->GetRadius(),
			player->GetPositionData());

		newPositon = collisionManager->HeadCollisionCheck(
			WorldSubSystem::GetInstance().GetSubSystem<CollisionObjectManager>()->GetCollisionObjects(),
			newPositon,
			player->GetVelocity(),
			player->GetPositionData(),
			player->GetRadius());

		actualPlayer->SetPos(newPositon);
	}
	else
	{
		collisionManager->Update(*player, actualPlayer->GetData());
		actualPlayer->ReceiveCollisionResult();
	}

	//座標更新
	actualPlayer->PositionUpdate();

	// プレイヤーデータの状態を確認し、変更があればオブザーバーに通知する
	StateConfirmation();

	TransparencyUpdate();
}

void PlayerManager::StartUpdate(const float& timer)
{
	actualPlayer->StartUpdate(timer);

	actualPlayer->PositionUpdate();
}

void PlayerManager::FinishUpdate(const float& timer)
{
	actualPlayer->FinishUpdate(timer);
	actualPlayer->PositionUpdate();
}

/// <summary>
/// 描画
/// </summary>
void PlayerManager::Draw()
{
	actualPlayer->Draw();
}

/// <summary>
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void PlayerManager::ResultCreate()
{
	auto self = shared_from_this();

	collisionManager = std::make_shared<CollisionManager>();

	player = std::make_shared<Player>();
	actualPlayer = std::dynamic_pointer_cast<Player>(player);
	player->ResultCreate();

	//サブシステムに追加
	WorldSubSystem::GetInstance().AddSubSystem<PlayerManager>(self);
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void PlayerManager::ResultInitialize()
{
	player->ResultInitialize();
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void PlayerManager::ResultUpdate()
{
	player->ResultUpdate();
}

/// <summary>
/// プレイヤーデータの状態を確認し、変更があれば通知する
/// </summary>
void PlayerManager::StateConfirmation()
{
	if (nowPlayerData != actualPlayer->GetData())
	{
		nowPlayerData = actualPlayer->GetData();
		NotifyStateChanged(nowPlayerData);
	}
}

/// @brief キャラとカメラの距離に応じて透過する
void PlayerManager::TransparencyUpdate()
{
	const float playerAndCameraDistance = WorldSubSystem::GetInstance().GetSubSystem<Camera>()->GetCameraAndTargetDistanceSize();
	const float kMaxCameraDistance = 20.0f;
	const float kMinCameraDistance = 10.0f;

	const float opacityRate = Calculation::CalculateBackProgress(
		kMaxCameraDistance,
		kMinCameraDistance,
		playerAndCameraDistance);

	//カメラとプレイヤーの距離が近づくにつれてキャラクターを透過する
	MV1SetOpacityRate(player->GetModelHandle(), opacityRate);
}

/// <summary>
/// 状態が変わったことを通知する
/// </summary>
/// <param name="amount"></param>
void PlayerManager::NotifyStateChanged(const PlayerData& playerData)
{
	//リスト内の要素を一つずつ調査する
	for (auto it = observers.begin(); it != observers.end(); )
	{
		//lockでweak_ptrからshared_ptrへ一時的に変換
		if (auto obs = it->lock())
		{
			obs->OnChangeState(playerData);
			++it;
		}
		else
		{
			// すでに破棄されている observer を削除
			it = observers.erase(it);
		}
	}
}

/// <summary>
/// オブザーバーの解除
/// </summary>
/// <param name="observer"></param>
void PlayerManager::RemoveObserver(std::shared_ptr<PlayerStateObserver> observer)
{
	// 削除対象か判定するラムダ関数を用意
	auto shouldRemove = [&](const std::weak_ptr<PlayerStateObserver>& weakObs)
		{
			// weak_ptrからshared_ptrを取得
			std::shared_ptr<PlayerStateObserver> locked = weakObs.lock();

			// 生きていて、かつobserverと同じオブジェクトならtrue
			if (locked)
			{
				return locked == observer;
			}

			// 期限切れまたは違うオブジェクトならfalse
			return false;
		};

	// remove_ifを呼んで削除対象の要素を末尾に移動させる
	auto newEnd = std::remove_if(observers.begin(), observers.end(), shouldRemove);

	// 実際にvectorの末尾の削除対象部分を削除する
	observers.erase(newEnd, observers.end());
}