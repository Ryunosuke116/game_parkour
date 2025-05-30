#pragma once
#include "PlayerStateActionBase.h"

class Jump : public PlayerStateActionBase
{
public:
	Jump(int& modelHandle,
		OldAnimState& oldAnimState, NowAnimState& nowAnimState);
	~Jump();

	bool MotionUpdate(PlayerData& playerData)override;

private:
	static constexpr float playAnimSpeed = 0.4f;	    // 移動速度

	bool isPush;		//ボタンを押したか
};

