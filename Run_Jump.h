#pragma once
#include "PlayerStateActionBase.h"

class runJump : public PlayerStateActionBase
{
public:
	runJump(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimState);
	~runJump();
	bool MotionUpdate(PlayerData& playerData)override;

private:

	static constexpr float playAnimSpeed = 0.6f;	    // ˆÚ“®‘¬“x

	bool isPush;		//ƒ{ƒ^ƒ“‚ð‰Ÿ‚µ‚½‚©
};

