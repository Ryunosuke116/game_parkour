#pragma once
#include "PlayerStateActionBase.h"

class Run : public PlayerStateActionBase
{
public:
<<<<<<< HEAD
	Run(int modelHandle, int& prevAttachIndex, NowAnimState nowAnimState);
	~Run();

	//void MotionUpdate()override;
=======
	Run(int modelHandle);
	~Run();

	void MotionUpdate()override;
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c

private:
	static constexpr float playAnimSpeed = 0.4f;	    // ˆÚ“®‘¬“x
};

