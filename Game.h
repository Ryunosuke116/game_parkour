#pragma once
#include "BaseManager.h"
#include "GameObjectManager.h"
#include "BlackOut.h"

class Game : public BaseScene
{
public:
	Game(SceneManager& manager);
	~Game();

	void Create();
	void Initialize();
	void Update();
	void Draw();

	void StartUpdate();

private:
	std::shared_ptr<BaseManager>		gameObjectManager			= NULL;
	std::shared_ptr<GameObjectManager>	gameObjectManager_actual	= NULL;
	std::shared_ptr<BlackOut>			blackOut					= NULL;
};

