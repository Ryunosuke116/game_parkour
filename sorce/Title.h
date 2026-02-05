#pragma once
#include "BaseManager.h"
#include "BlackOut.h"
#include <memory>
#include <vector>

class TitleObjectManager;
class ObjectManager;

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

	bool isPush;
	std::shared_ptr<ObjectManager>	objectManager;
};

