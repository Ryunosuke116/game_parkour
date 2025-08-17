#pragma once
#include "BaseScene.h"
#include "BaseUI.h"
#include "BlackOut.h"
#include <nlohmann/json.hpp>

class Result : public BaseScene
{
public:
	Result(SceneManager& manager);
	~Result();

	void Create();
	void Initialize();
	void Update();
	void Draw();

	void Add(std::shared_ptr<BaseUI> ui,
		nlohmann::json& jsonData);

	std::string GetJsonTag() { return jsonTag; }

private:

	std::vector<std::shared_ptr<BaseUI>> ui_list;

	int modelHandle;
	int backGroundHandle;
	int coinHandle;
	int numberHandle[10];
	int crossHandle;
	int rankHandle;
	int coin_x;
	int coin_y;


	std::string num;
	std::string jsonTag;

	bool isPush;

	std::shared_ptr<BlackOut> blackOut = NULL;
};
