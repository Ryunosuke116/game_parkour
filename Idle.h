#pragma once
#include "PlayerStateActionBase.h"

class Player;

class Idle : public PlayerStateActionBase
{
public:
	Idle(int& modelHandle, AnimState& oldAnimState,
		AnimState& nowAnimState, PlayerData& playerData);
	~Idle();

	void Initialize(int& modelHandle, Player& player)override;

	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)override;
	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	void Enter(PlayerData& playerData) override;		//ó‘Ô‚É“ü‚Á‚½‚Æ‚«
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«

private:
	static constexpr float playAnimSpeed = 0.4f;	    // ˆÚ“®‘¬“x
};

