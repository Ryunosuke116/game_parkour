#pragma once
#include "nlohmann/json.hpp"

class FinishCut
{
public:
	FinishCut();
	~FinishCut();

	void Initialize();
	bool Update();
	void Draw();
	void Load(const nlohmann::json& jsonData);

	std::string GetTag() { return tag; }

	void SetIsDrawFinish(const bool set) { isDrawFinish = set; }

private:
	int finishHandle;

	bool isDrawFinish;

	float finishGraphTimer;				//スタート画像の挙動タイマー
	float finishGraphPosX;
	float finishGraphPosY;

	std::string tag;
	nlohmann::json	jsonData;
};

