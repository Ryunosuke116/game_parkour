#pragma once
#include "BaseObject.h"

class GameTimer : public BaseObject
{
public:
	GameTimer();
	~GameTimer();

	void Initialize()override;
	void Update()override;
	bool Draw()override;

private:
	int time;
	int setTime;
	int sec;        //•b
	int min;        //•ª
	int msec;       //ƒ~ƒŠ•b
};

