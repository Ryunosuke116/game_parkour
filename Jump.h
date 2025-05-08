#pragma once
#include "PlayerStateActionBase.h"

class Jump : public PlayerStateActionBase
{
public:
	Jump(int modelHandle);
	~Jump();

	void  MotionUpdate()override;

private:
	static constexpr float playAnimSpeed = 0.4f;	    // ˆÚ“®‘¬“x
};

