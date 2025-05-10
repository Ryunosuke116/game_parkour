#pragma once
#include "PlayerStateActionBase.h"

class Jump : public PlayerStateActionBase
{
public:
<<<<<<< HEAD
	Jump(int modelHandle, int& prevAttachIndex, NowAnimState nowAnimState);
	~Jump();

	//void  MotionUpdate()override;
=======
	Jump(int modelHandle);
	~Jump();

	void  MotionUpdate()override;
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c

private:
	static constexpr float playAnimSpeed = 0.4f;	    // ˆÚ“®‘¬“x
};

