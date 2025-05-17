#pragma once
#include "PlayerStateActionBase.h"

class Falling_Idle : public PlayerStateActionBase
{
public:
	Falling_Idle(int& modelHandle,
		OldAnimState& oldAnimState, NowAnimState& nowAnimState);
	~Falling_Idle();

	bool  MotionUpdate(PlayerData& playerData)override;

private:
	static constexpr float playAnimSpeed = 0.4f;	    // ˆÚ“®‘¬“x
};

