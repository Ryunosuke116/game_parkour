#pragma once
#include "BaseUI.h"

class GameTimer : public BaseUI
{
public:
	GameTimer();
	~GameTimer();

	void Load(const nlohmann::json& jsonData)		override;
	void Initialize()								override;
	void Update()									override;
	void Draw()										override;
	void Create()									override;
	void Add()										override {}
	void ResultCreate()								override {}
	void ResultInitialize()							override {}
	void ResultUpdate()								override {}

	bool IsUpdateMin();

	int TimeForciblyZero(const int maxSec);

	bool IsFinish();

	std::string CreateCountNumber(const int time);

	void ResetSetTime() { setTime = GetNowCount(); }
private:
	int time;
	int setTime;
	int secTime;				//•b	
	int minTime;				//•ª
	int numberHandle[10];
	int colonHandle;	
	int redBright;			//Ô‚Ì•`‰æ‹P“x
	int greenBright;		//—Î‚Ì•`‰æ‹P“x
	int blueBright;			//Â‚Ì•`‰æ‹P“x
	int addBright;

	float numPosX;
	float numPosY;
	float numWidth;
	float numHeight;
	float colonPosX;

	bool isUpdateMin;	

	std::string countNumberSec;
	std::string countNumberMin;

	static constexpr int kMaxBright = 255;
	static constexpr int kMinBright = 0;
};

