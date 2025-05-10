#pragma once
#include "PlayerStateActionBase.h"

class Falling_Idle : public PlayerStateActionBase
{
public:
<<<<<<< HEAD
	Falling_Idle(int modelHandle, int& prevAttachIndex, NowAnimState nowAnimState);
=======
	Falling_Idle(int modelHandle);
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c
	~Falling_Idle();

	void  MotionUpdate()override;

private:
	static constexpr float playAnimSpeed = 0.4f;	    // ˆÚ“®‘¬“x
};

