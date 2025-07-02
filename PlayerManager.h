#pragma once
class PlayerManager
{
public:

	PlayerManager();
	~PlayerManager();

	void Update(int mapHandle,Player& player);
	bool Draw();

private:
	std::shared_ptr<CollisionManager> collisionManager = NULL;
};

