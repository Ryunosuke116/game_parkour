#pragma once
class PlayerManager
{
public:

	PlayerManager();
	~PlayerManager();

	void Initialize();
	void Update(int mapHandle,const VECTOR& cameraDirection);
	bool Draw();

	struct 

private:
	std::shared_ptr<CollisionManager> collisionManager = NULL;

	std::shared_ptr<BaseChara> player = NULL;
	std::shared_ptr<Player> actualPlayer = NULL;
};

