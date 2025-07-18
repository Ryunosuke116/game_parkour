#pragma once
#include "PlayerStateActionBase.h"
class Running_turn :
    public PlayerStateActionBase
{
public:
	Running_turn(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimStatel, PlayerData& playerData);
	~Running_turn();

	bool MotionUpdate(PlayerData& playerData)override;

private:
	static constexpr float playAnimSpeed = 0.6f;	    // ˆÚ“®‘¬“x

};

