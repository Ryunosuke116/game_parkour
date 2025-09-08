#pragma once
#include "PlayerStateBase.h"

class Player;

class QuickRoll : public PlayerStateBase
{
public:
	QuickRoll(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimState);
	~QuickRoll();

	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)override;

	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	bool MotionUpdate(PlayerData& playerData)override;
	void Enter(PlayerData& playerData) override;		//ó‘Ô‚É“ü‚Á‚½‚Æ‚«
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«
private:
	static constexpr float playAnimSpeed = 0.9f;	    // ˆÚ“®‘¬“x

	bool isPush;		//ƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚½‚©
	bool isRun;
	bool isIdle;
};