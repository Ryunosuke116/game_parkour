#pragma once
#include "BaseObject.h"
#include <nlohmann/json.hpp>

class GameTimer : public BaseObject
{
public:
	GameTimer();
	~GameTimer();

	void Load(const nlohmann::json& jsonData)override{}
	void Initialize()override;
	void Update()override;
	void Draw()override;
	

private:
	int time;
	int setTime;
	int sec;        //•b
	int min;        //•ª
	int msec;       //ƒ~ƒŠ•b
};

