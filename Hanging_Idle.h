#pragma once
#include "PlayerStateActionBase.h"
class Hanging_Idle : public PlayerStateActionBase
{
public:
	Hanging_Idle(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimStatel, PlayerData& playerData);
	~Hanging_Idle();

	//bool MotionUpdate(PlayerData& playerData)override;

private:
	static constexpr float playAnimSpeed = 0.4f;	    // アニメーション速度
};

