#pragma once
#include "PlayerStateActionBase.h"

class Run : public PlayerStateActionBase
{
public:
	Run(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimState);
	~Run();

	//void MotionUpdate()override;

private:
	static constexpr float playAnimSpeed = 0.4f;	    // ˆÚ“®‘¬“x
};

