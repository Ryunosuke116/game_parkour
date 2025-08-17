#pragma once
#include "Object_interface.h"

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

	void SetIsDraw_finish(const bool set) { isDraw_finish = set; }

private:
	int finishHandle;
	int x_finishGraph;
	int y_finishGraph;

	bool isDraw_finish;

	float finishGraph_timer;				//スタート画像の挙動タイマー

	std::string tag;
	nlohmann::json	jsonData;
};

