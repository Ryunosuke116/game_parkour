#pragma once
#include "PlayerStateActionBase.h"

class Falling_Idle : public PlayerStateActionBase
{
public:
	Falling_Idle(int modelHandle);
	~Falling_Idle();

	void  MotionUpdate()override;

private:
	static constexpr float playAnimSpeed = 0.4f;	    // ˆÚ“®‘¬“x
};

