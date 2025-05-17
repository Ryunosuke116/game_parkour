#pragma once
#include "PlayerStateActionBase.h"

class Quick_Roll : public PlayerStateActionBase
{
public:
	Quick_Roll(int& modelHandle,
		OldAnimState& oldAnimState, NowAnimState& nowAnimState);
	~Quick_Roll();

	bool MotionUpdate(PlayerData& playerData)override;

private:
	static constexpr float playAnimSpeed = 1.0f;	    // �ړ����x

	bool isPush;		//�{�^������������
};