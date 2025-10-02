#pragma once
#include "PlayerStateBase.h"

class Player;

class AnimationChanger
{
public:
	AnimationChanger();
	~AnimationChanger();

	std::shared_ptr<PlayerStateBase> ChangeState(const int modelHandle,
		Player& player, PlayerData& playerData,
		std::shared_ptr<PlayerStateBase>& nowState);

	void SetOldAnimState(PlayerStateBase::AnimState animState);
	void SetNowAnimState(PlayerStateBase::AnimState animState);
	void Create(const int modelHandle);
	void Initialize(const int& num, 
		const int modelHandle,
		std::shared_ptr<PlayerStateBase>& nowState,
		PlayerData& playerData,
		Player& player);

	void ResultInitialize(const int num, 
		const int modelHandle,
		std::shared_ptr<PlayerStateBase>& nowState,
		PlayerData& playerData,
		Player& player);

	int NowGetAnimNumber() { return nowAnimNumber; }

private:
	int nowAnimNumber;

	std::unordered_map<int, std::shared_ptr<PlayerStateBase>> stateList;

	PlayerStateBase::AnimState oldAnimState;
	PlayerStateBase::AnimState nowAnimState;
};

