#pragma once
#include "PlayerStateActionBase.h"

class Braced_Hang_To_Crouch : public PlayerStateActionBase
{
public:
	Braced_Hang_To_Crouch(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimState, PlayerData& playerData);
	~Braced_Hang_To_Crouch();

private:
	static constexpr float playAnimSpeed = 0.4f;	    // アニメーション速度

};

