#pragma once
#include "PlayerStateActionBase.h"

class Run : public PlayerStateActionBase
{
public:
	Run(int& modelHandle,
		OldAnimState& oldAnimState, NowAnimState& nowAnimState);
	~Run();

	//void MotionUpdate()override;

private:
	static constexpr float playAnimSpeed = 0.4f;	    // �ړ����x
};

