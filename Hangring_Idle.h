#pragma once
#include "PlayerStateActionBase.h"
class Hangring_Idle : public PlayerStateActionBase
{
public:
	Hangring_Idle(int& modelHandle,
		OldAnimState& oldAnimState, NowAnimState& nowAnimStatel, PlayerData& playerData);
	~Hangring_Idle();

private:
	static constexpr float playAnimSpeed = 0.4f;	    // アニメーション速度
};

