#pragma once
class PlayerManager
{
public:

	PlayerManager();
	~PlayerManager();

	void Update(int mapHandle,Player& player);
	void Draw();

private:
	std::shared_ptr<CollisionManager> collisionManager = NULL;
};

