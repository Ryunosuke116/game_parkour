#pragma once
#include "PlayerStateActionBase.h"

class Idle : public PlayerStateActionBase
{
public:
	Idle(int& modelHandle, 
		OldAnimState& oldAnimState, NowAnimState& nowAnimState);
	~Idle();

	//void  MotionUpdate()override;

private:
	static constexpr float playAnimSpeed = 0.4f;	    // ˆÚ“®‘¬“x
};

