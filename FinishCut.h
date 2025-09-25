#pragma once
#include "IObject.h"

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
	int x_finishGraph;
	int y_finishGraph;

	bool isDrawFinish;

	float finishGraphTimer;				//スタート画像の挙動タイマー

	std::string tag;
	nlohmann::json	jsonData;
};

