#pragma once
#include "PlayerStateActionBase.h"

class Idle_To_Sprint : public PlayerStateActionBase
{
public:
	Idle_To_Sprint(int& modelHandle,
		OldAnimState& oldAnimState, NowAnimState& nowAnimState);
	~Idle_To_Sprint();

	bool MotionUpdate(PlayerData& playerData)override;

private:
	static constexpr float playAnimSpeed = 0.6f;	    // ˆÚ“®‘¬“x
};

