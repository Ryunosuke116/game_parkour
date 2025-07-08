#pragma once
class PlayerManager
{
private:
	std::shared_ptr<CollisionManager> collisionManager = NULL;

	std::shared_ptr<BaseChara> player = NULL;
	std::shared_ptr<Player> actualPlayer = NULL;
public:

	PlayerManager();
	~PlayerManager();

	void Initialize();
	void Update(int mapHandle,const VECTOR& cameraDirection);
	bool Draw();
	VECTOR PositionCheck(const VECTOR& hangingPos, const VECTOR& playerPos);

	std::shared_ptr<Player> GetPlayer()const { return actualPlayer; }

};

