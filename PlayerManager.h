#pragma once
#include "BaseGameObjectManager.h"
#include "HitCheck.h"

class PlayerManager : public BaseGameObjectManager
{
private:
	std::vector<std::shared_ptr<BaseChara>> characters;
	std::shared_ptr<CollisionManager> collisionManager;

	std::shared_ptr<BaseChara> player = NULL;
	std::shared_ptr<Player> actualPlayer = NULL;
	AABB playerAABB;
public:

	PlayerManager();
	~PlayerManager();
	
	void Update(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
		const VECTOR& cameraDirection);

	void Create()		override;
	void Initialize()	override;
	void Update()		override;
	void Draw()			override;
	void Add()			override;

	VECTOR GetPosition() { return player->GetPosition(); }
	float GetAngle() { return player->GetAngle(); }
	
	VECTOR PositionCheck(const VECTOR& hangingPos, const VECTOR& playerPos);

	std::shared_ptr<Player> GetPlayer() { return actualPlayer; }
	AABB GetPlayerAABB() { return playerAABB; }

	/*std::shared_ptr<Player> GetPlayer() const {
		for (auto& chara : characters)
		{
			if (auto player = std::dynamic_pointer_cast<Player>(chara))
			{
				return player;
			}
		}
		return nullptr;
	}*/
};

