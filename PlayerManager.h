#pragma once
#include "BaseManager.h"

class PlayerManager : public BaseManager
{
private:
	std::shared_ptr<CollisionManager> collisionManager = NULL;

	std::shared_ptr<BaseChara> player = NULL;
	std::shared_ptr<Player> actualPlayer = NULL;
public:

	PlayerManager();
	~PlayerManager();

	void Initialize()override;
	void Update(int mapHandle,const VECTOR& cameraDirection);
	bool Draw()override;
	void Create()override;
	void Add()override;

	VECTOR PositionCheck(const VECTOR& hangingPos, const VECTOR& playerPos);

	std::shared_ptr<Player> GetPlayer()const { return actualPlayer; }

	void Update()override;


};

