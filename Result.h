#pragma once
#include "BaseScene.h"
#include "BlackOut.h"

class Result : public BaseScene
{
public:
	Result(SceneManager& manager);
	~Result();

	void Initialize();
	void Update();
	void Draw();
private:
	int modelHandle;

	bool isPush;

	std::shared_ptr<BlackOut> blackOut = NULL;
};
