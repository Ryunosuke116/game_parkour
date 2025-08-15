#pragma once
#include "BaseScene.h"
#include "BlackOut.h"

class Result : public BaseScene
{
public:
	Result(SceneManager& manager);
	~Result();

	void Create();
	void Initialize();
	void Update();
	void Draw();
private:
	int modelHandle;
	int coinHandle;
	int numberHandle[10];
	int crossHandle;
	int rankHandle;
	int coin_x;
	int coin_y;


	std::string num;


	bool isPush;

	std::shared_ptr<BlackOut> blackOut = NULL;
};
