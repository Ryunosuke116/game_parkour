#pragma once
#include "PlayerStateActionBase.h"

class Player;

class Run_To_Stop : public PlayerStateActionBase
{
public:
	Run_To_Stop(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimState);
	~Run_To_Stop();

	void Initialize(int& modelHandle, Player& player)override;

	bool MotionUpdate(PlayerData& playerData)override;
	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::shared_ptr<BaseObject>>& collisionObjects, Player& player)override;

	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	void Enter(PlayerData& playerData) override;		//ó‘Ô‚É“ü‚Á‚½‚Æ‚«
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«
private:
	static constexpr float playAnimSpeed = 0.7f;	    // ˆÚ“®‘¬“x
};

