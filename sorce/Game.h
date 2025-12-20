#pragma once
#include "BaseManager.h"
#include "ObjectManager.h"
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
	std::shared_ptr<BaseManager>	gameObjectManager			= NULL;
	std::shared_ptr<ObjectManager>	gameObjectManager_actual	= NULL;
};

