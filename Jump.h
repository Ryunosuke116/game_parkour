#pragma once
#include "PlayerStateActionBase.h"

class Jump : public PlayerStateActionBase
{
public:
	Jump(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimState);
	~Jump();

	bool MotionUpdate(PlayerData& playerData)override;

private:
	static constexpr float playAnimSpeed = 0.4f;	    // ˆÚ“®‘¬“x

	bool isPush;		//ƒ{ƒ^ƒ“‚ð‰Ÿ‚µ‚½‚©
};

