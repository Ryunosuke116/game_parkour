#pragma once
#include "PlayerStateActionBase.h"

class Player;

class AnimationChanger
{
public:

	AnimationChanger(std::shared_ptr<ISoundPlayer> sound);
	~AnimationChanger();

	std::shared_ptr<PlayerStateActionBase> ChangeState(int& modelHandle,
		Player& player, PlayerData& playerData,
		std::shared_ptr<PlayerStateActionBase>& nowState);

	void SetOldAnimState(PlayerStateActionBase::AnimState animState);
	void SetNowAnimState(PlayerStateActionBase::AnimState animState);
	void Initialize(const int& num, int& modelHandle,
		std::shared_ptr<PlayerStateActionBase>& nowState,
		PlayerData& playerData, Player& player);
	int GetAnimNumber_now() { return animNumber_Now; }


private:

	int animNumber_Now;
	int soundHandle;

	PlayerStateActionBase::AnimState oldAnimState;
	PlayerStateActionBase::AnimState nowAnimState;
	std::shared_ptr<ISoundPlayer> soundPlayer;
};

