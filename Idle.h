#pragma once
#include "PlayerStateActionBase.h"

class Idle : public PlayerStateActionBase
{
public:
<<<<<<< HEAD
	Idle(int modelHandle, int& prevAttachIndex, NowAnimState nowAnimState);
	~Idle();

	//void  MotionUpdate()override;
=======
	Idle(int modelHandle);
	~Idle();

	void  MotionUpdate()override;
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c

private:
	static constexpr float playAnimSpeed = 0.4f;	    // ˆÚ“®‘¬“x
};

