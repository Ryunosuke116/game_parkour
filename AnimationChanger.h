#pragma once
#include "PlayerStateActionBase.h"

class AnimationChanger
{
public:

	AnimationChanger();
	~AnimationChanger();

	void ChangeState(int& modelHandle, const bool& isChangeState,
		PlayerStateActionBase::PlayerData& playerData,
		std::shared_ptr<PlayerStateActionBase>& nowState);
	void SetOldAnimState(PlayerStateActionBase::AnimState animState);
	void SetNowAnimState(PlayerStateActionBase::AnimState animState);

private:

	int animNumber_Now;

	PlayerStateActionBase::AnimState oldAnimState;
	PlayerStateActionBase::AnimState nowAnimState;
};

