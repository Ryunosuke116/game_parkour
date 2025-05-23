#pragma once
#include "PlayerStateActionBase.h"

class Run_Jump : public PlayerStateActionBase
{
public:
	Run_Jump(int& modelHandle,
		OldAnimState& oldAnimState, NowAnimState& nowAnimState);
	~Run_Jump();
	bool MotionUpdate(PlayerData& playerData)override;

private:

	static constexpr float playAnimSpeed = 0.6f;	    // 移動速度

	bool isPush;		//ボタンを押したか
};

