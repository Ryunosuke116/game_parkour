#pragma once
#include "PlayerStateBase.h"

class Player;

class FallingIdle : public PlayerStateBase
{
public:
	FallingIdle(const int modelHandle);
	~FallingIdle();

	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)			override;
	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)	override;
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«
private:
	static constexpr float kPlayAnimSpeed = 0.4f;	    // ˆÚ“®‘¬“x
};

