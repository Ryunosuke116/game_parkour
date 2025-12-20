#pragma once
#include "PlayerStateBase.h"

class Player;

class QuickRoll : public PlayerStateBase
{
public:
	QuickRoll(const int modelHandle);
	~QuickRoll();

	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)override;

	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	bool MotionUpdate(PlayerData& playerData)override;
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«
private:
	static constexpr float kPlayAnimSpeed = 0.9f;	    // ˆÚ“®‘¬“x

	bool isPush;		//ƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚½‚©
	bool isRun;
	bool isIdle;
};