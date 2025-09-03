#pragma once
#include "BaseManager.h"
#include "BlackOut.h"
#include <memory>
#include <vector>

class TitleObjectManager;

class Title : public BaseScene
{
public:
	Title(SceneManager& manager);
	~Title();

	void Create();
	void Initialize();
	void Update();
	void Draw();
private:
	int modelHandle;
	int soundHandle;
	int buttonSound;
	int titleHandle;

	bool isPush;
	std::shared_ptr<TitleObjectManager>	objectManager;
};

