#pragma once
#include "PlayerStateBase.h"

class Player;

class AnimationChanger
{
public:
	AnimationChanger();
	~AnimationChanger();

	std::shared_ptr<PlayerStateBase> ChangeState(int& modelHandle,
		Player& player, PlayerData& playerData,
		std::shared_ptr<PlayerStateBase>& nowState);

	void SetOldAnimState(PlayerStateBase::AnimState animState);
	void SetNowAnimState(PlayerStateBase::AnimState animState);
	void Initialize(const int& num, 
		int& modelHandle,
		std::shared_ptr<PlayerStateBase>& nowState,
		PlayerData& playerData,
		Player& player);

	void ResultInitialize(const int& num, 
		int& modelHandle,
		std::shared_ptr<PlayerStateBase>& nowState,
		PlayerData& playerData,
		Player& player);

	int GetAnimNumber_now() { return animNumber_Now; }

private:
	int animNumber_Now;

	PlayerStateBase::AnimState oldAnimState;
	PlayerStateBase::AnimState nowAnimState;
};

