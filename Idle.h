#pragma once
#include "PlayerStateActionBase.h"

class Idle : public PlayerStateActionBase
{
public:
	Idle(int modelHandle);
	~Idle();

	void  MotionUpdate()override;

private:
	static constexpr float playAnimSpeed = 0.4f;	    // ˆÚ“®‘¬“x
};

