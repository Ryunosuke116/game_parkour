#pragma once
#include "BaseGameObjectManager.h"
#include "HitCheck.h"
#include "PlayerStateObserver.h"
#include "ISoundPlayer.h"
#include "BaseChara.h"

class CollisionManager;
class EffectManager;
class BaseObject;
class Player;

class PlayerManager :
	public BaseGameObjectManager,
	public std::enable_shared_from_this<PlayerManager>
{
public:
	PlayerManager();
	~PlayerManager();
	
	void Update_start(const float& timer);
	void Update_finish(const float& timer);

	void Create()		override;
	void Initialize()	override;
	void Update()		override;
	void Draw()			override;
	void Add()			override{}
	void ResultCreate()override;
	void ResultInitialize()override;
	void ResultUpdate()override;
	
	VECTOR PositionCheck(const VECTOR& hangingPos, const VECTOR& playerPos);
	void StateConfirmation();

	//ゲッター
	VECTOR GetPosition()const { return player->GetPosition(); }
	float GetRadian()const { return player->GetRadian(); }
	float GetDegree()const { return player->GetDegree(); }
	std::shared_ptr<Player> GetPlayer()const { return actualPlayer; }
	int GetCoinCount()const { return player->GetCoinCount(); }

	//オブサーバー関連
	void AddObserver(std::shared_ptr<PlayerStateObserver> observer) { observers.push_back(observer); }
	void RemoveObserver(std::shared_ptr<PlayerStateObserver> observer);
	void NotifyStateChanged(const PlayerData& playerData);

private:
	std::shared_ptr<CollisionManager> collisionManager;
	std::vector<std::weak_ptr<PlayerStateObserver>> observers;

	std::shared_ptr<BaseChara> player = NULL;
	std::shared_ptr<Player> actualPlayer = NULL;
	PlayerData now_playerData;
};

