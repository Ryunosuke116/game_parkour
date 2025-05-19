#pragma once
#include "PlayerStateActionBase.h"

class Run_To_Stop : public PlayerStateActionBase
{
public:
	Run_To_Stop(int& modelHandle,
		OldAnimState& oldAnimState, NowAnimState& nowAnimState);
	~Run_To_Stop();

	bool MotionUpdate(PlayerData& playerData)override;

private:
	static constexpr float playAnimSpeed = 0.7f;	    // ˆÚ“®‘¬“x
};

