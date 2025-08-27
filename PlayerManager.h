#pragma once
#include "BaseGameObjectManager.h"
#include "HitCheck.h"
#include "PlayerStateObserver.h"
#include "ISoundPlayer.h"

class EffectManager;
class BaseObject;

class PlayerManager : public BaseGameObjectManager
{
public:
	PlayerManager();
	~PlayerManager();
	
	void Update_start(const float& timer);
	void Update_finish(const float& timer);

	void Create()		override;
	void Initialize()	override;
	void Update(ObjectMediator& objectMediator)		override;
	void Draw()			override;
	void Add()			override;
	
	VECTOR PositionCheck(const VECTOR& hangingPos, const VECTOR& playerPos);
	void StateConfirmation();

	//ゲッター
	VECTOR GetPosition() { return player->GetPosition(); }
	float GetAngle() { return player->GetAngle(); }
	std::shared_ptr<Player> GetPlayer()const { return actualPlayer; }
	AABB GetPlayerAABB() { return playerAABB; }
	int GetCoinCount()const { return player->GetCoinCount(); }

	//オブサーバー関連
	void AddObserver(std::shared_ptr<PlayerStateObserver> observer) { observers.push_back(observer); }
	void RemoveObserver(std::shared_ptr<PlayerStateObserver> observer);
	void NotifyStateChanged(const PlayerData& playerData);

private:
	std::vector<std::shared_ptr<BaseChara>> characters;
	std::shared_ptr<CollisionManager> collisionManager;
	std::vector<std::weak_ptr<PlayerStateObserver>> observers;

	std::shared_ptr<BaseChara> player = NULL;
	std::shared_ptr<Player> actualPlayer = NULL;
	AABB playerAABB;
	PlayerData now_playerData;
};

