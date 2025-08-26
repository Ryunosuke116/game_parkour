#pragma once
#include "PlayerStateBase.h"

class Player;

class Run_To_Stop : public PlayerStateBase
{
public:
	Run_To_Stop(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimState, std::shared_ptr<ISoundPlayer> sound);
	~Run_To_Stop();

	bool MotionUpdate(PlayerData& playerData)override;
	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)override;

	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	void Enter(PlayerData& playerData) override;		//ó‘Ô‚É“ü‚Á‚½‚Æ‚«
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«
private:
	static constexpr float playAnimSpeed = 0.7f;	    // ˆÚ“®‘¬“x
};

