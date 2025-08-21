#pragma once
#include "BaseUI.h"
#include <nlohmann/json.hpp>

class GameTimer : public BaseUI
{
public:
	GameTimer();
	~GameTimer();

	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	
	std::string CreateCountNumber(const int time);

	void ResetSetTime(){ setTime = GetNowCount(); }

	bool IsUpdateMin();

	int TimeForciblyZero(const int maxSec);

	bool IsFinish();

private:
	int time;
	int setTime;
	int sec;        //秒
	int min;        //分
	int msec;       //ミリ秒
	int positionX;
	int positionY;

	bool isUpdateMin;		//分タイマーを更新するか

	int numberHandle[10];
	std::string countNumberSec;
	std::string countNumberMin;
};

