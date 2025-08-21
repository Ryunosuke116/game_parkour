#pragma once
#include "PlayerStateActionBase.h"

class Player;

class Quick_Roll : public PlayerStateActionBase
{
public:
	Quick_Roll(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimState, std::shared_ptr<ISoundPlayer> sound);
	~Quick_Roll();

	void Initialize(int& modelHandle, Player& player)override;

	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)override;

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