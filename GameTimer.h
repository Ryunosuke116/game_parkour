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
	void Create()override {}
	void Add()override{}
	void ResultCreate()override{}
	void ResultInitialize()override{}
	void ResultUpdate()override{}

	bool IsUpdateMin();

	int TimeForciblyZero(const int maxSec);

	bool IsFinish();

	std::string CreateCountNumber(const int time);

	void ResetSetTime() { setTime = GetNowCount(); }
private:
	int time;
	int setTime;
	int sec;        //•b
	int min;        //•ª
	int msec;       //ƒ~ƒŠ•b
	int positionX;
	int positionY;

	bool isUpdateMin;	

	int numberHandle[10];
	std::string countNumberSec;
	std::string countNumberMin;
};

