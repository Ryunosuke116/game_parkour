#pragma once
#include "PlayerStateBase.h"

class Player;

class Falling_Idle : public PlayerStateBase
{
public:
	Falling_Idle(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimState);
	~Falling_Idle();

	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)override;
	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	void Enter(PlayerData& playerData) override;		//ó‘Ô‚É“ü‚Á‚½‚Æ‚«
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«
private:
	static constexpr float playAnimSpeed = 0.4f;	    // ˆÚ“®‘¬“x
	static constexpr float cliff_radius = 4.0f;
};

