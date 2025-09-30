#pragma once
#include "IObject.h"

class Tutorial 
{
public:
	Tutorial();
	~Tutorial();

	void Initialize();
	bool Update();
	void Draw(const float& streamStartPictureTimer);
	void Load(const nlohmann::json& jsonData);

	std::string GetTag() { return tag; }
private:
	int tutorialHandle;
	int startHandle;
	int tutorialGraphPosX;
	int tutorialGraphPosY;
	int startGraphPosX;
	int startGraphPosY;

	bool isPushStart;

	float startGraphTimer;				//スタート画像の挙動タイマー

	std::string tag;
	nlohmann::json	jsonData;
};

