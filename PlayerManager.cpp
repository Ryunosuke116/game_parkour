#include "common.h"
#include <vector>
#include "Player.h"
#include "CollisionManager.h"
#include "EffectManager.h"
#include "PlayerManager.h"
#include "Calculation.h"
#include "BaseObject.h"
#include "DebugDrawer.h"
#include "SubSystemManager.h"
#include "WorldSubSystem.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerManager::PlayerManager():
	BaseGameObjectManager(),
	now_playerData({false})
{
	tag = "player";
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerManager::~PlayerManager()
{

}

void PlayerManager::Create() 
{
	auto self = shared_from_this();

	collisionManager = std::make_shared<CollisionManager>();

	player = std::make_shared<Player>();
	actualPlayer = std::dynamic_pointer_cast<Player>(player);
	player->Create();

	//サブシステムに追加
	WorldSubSystem::Instance().AddSubSystem<PlayerManager>(self);
}

/// <summary>
/// 初期化
/// </summary>
void PlayerManager::Initialize()
{
	actualPlayer->Initialize();

	now_playerData = { false };
}

/// <summary>
/// 更新
/// </summary>
/// <param name="mapHandle"></param>
/// <param name="player"></param>
void PlayerManager::Update()
{
	actualPlayer->Update();

	collisionManager->Update(*player, actualPlayer->GetData());
	
	actualPlayer->Receive_CollisionResult();

	actualPlayer->PositionUpdate();

	StateConfirmation();

}

void PlayerManager::Update_start(const float& timer)
{
	actualPlayer->Update_start(timer);

	actualPlayer->PositionUpdate();

}

void PlayerManager::Update_finish(const float& timer)
{
	actualPlayer->Update_finish(timer);
	actualPlayer->PositionUpdate();
}

/// <summary>
/// 描画
/// </summary>
void PlayerManager::Draw()
{
	actualPlayer->Draw();

	collisionManager->Draw();
}

void PlayerManager::StateConfirmation()
{
	if (now_playerData != actualPlayer->GetData())
	{
		now_playerData = actualPlayer->GetData();
		NotifyStateChanged(now_playerData);
	}
}

VECTOR PlayerManager::PositionCheck(const VECTOR& hangingPos, const VECTOR& playerPos)
{
	VECTOR newPlayerPos = playerPos;
	VECTOR hanging = hangingPos;
	hanging.y = playerPos.y;

	Calculation::Leap(newPlayerPos, hanging, 0.1f);

	return newPlayerPos;
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
	auto shouldRemove = [&](const std::weak_ptr<PlayerStateObserver>& weakObs) {
		
		// weak_ptrからshared_ptrを取得
		std::shared_ptr<PlayerStateObserver> locked = weakObs.lock();

		// 生きていて、かつobserverと同じオブジェクトならtrue
		if (locked) {
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