#pragma once
#include "IObject.h"

class Tutorial 
{
public:
	Tutorial();
	~Tutorial();

	void Initialize();
	bool Update();
	void Draw(const float& stream_startPicture_timer);
	void Load(const nlohmann::json& jsonData);

	std::string GetTag() { return tag; }
private:
	int tutorialHandle;
	int startHandle;
	int x_tutorialGraph;
	int y_tutorialGraph;
	int x_startGraph;
	int y_startGraph;

	bool isPush_start;

	float startGraph_timer;				//スタート画像の挙動タイマー

	std::string tag;
	nlohmann::json	jsonData;
};

