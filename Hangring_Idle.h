#pragma once
#include "PlayerStateActionBase.h"
class Hangring_Idle : public PlayerStateActionBase
{
public:
	Hangring_Idle(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimStatel, PlayerData& playerData);
	~Hangring_Idle();

	//bool MotionUpdate(PlayerData& playerData)override;

private:
	static constexpr float playAnimSpeed = 0.4f;	    // アニメーション速度
};

