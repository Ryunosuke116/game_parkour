#pragma once
#include "BaseManager.h"
#include "BlackOut.h"

class Title : public BaseScene
{
public:
	Title(SceneManager& manager);
	~Title();

	void Initialize();
	void Update();
	void Draw();
private:
	int modelHandle;

	bool isPush;

	std::shared_ptr<BlackOut> blackOut = NULL;
};

