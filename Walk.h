#pragma once
#include "PlayerStateBase.h"

class Player;

class Walk : public PlayerStateBase
{
public:

	Walk(const int modelHandle);
	~Walk();

	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)override;

	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«
	VECTOR Move(const VECTOR& cameraDirection, PlayerData& playerData)override;

private:
	static constexpr float kPlayAnimSpeed = 0.5f;	    // ˆÚ“®‘¬“x

	float differenceDegree;
	float stopTime;				//‰½f~‚Ü‚Á‚Ä‚¢‚é‚©
	float angle;
	float playerMoveSpeed;
	float playerMoveSpeed_max;
	float degree_new;
};

