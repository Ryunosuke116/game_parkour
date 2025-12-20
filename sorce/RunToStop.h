#pragma once
#include "PlayerStateBase.h"

class Player;

class RunToStop : public PlayerStateBase
{
public:
	RunToStop(const int modelHandle);
	~RunToStop();

	bool MotionUpdate(PlayerData& playerData)override;
	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)override;

	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«
private:
	static constexpr float kPlayAnimSpeed = 0.7f;	    // ˆÚ“®‘¬“x
};

