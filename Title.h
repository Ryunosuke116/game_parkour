#pragma once
#include "BaseManager.h"

class Title : public BaseScene
{
	Title(SceneManager& manager);
	~Title();

	void Initialize();
	void Update();
	void Draw();
private:

	int modelHandle;
};

