#pragma once
#include "BaseManager.h"
#include "GameObjectManager.h"

class Game : public BaseScene
{
public:
	Game(SceneManager& manager);
	~Game();

	void Initialize();
	void Update();
	void Draw();

private:
	std::shared_ptr<BaseManager>		gameObjectManager			= NULL;
	std::shared_ptr<GameObjectManager>	gameObjectManager_actual	= NULL;
};

