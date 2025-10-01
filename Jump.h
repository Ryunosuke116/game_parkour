#pragma once
#include "PlayerStateBase.h"

class Player;

class Jump : public PlayerStateBase
{
public:
	Jump(const int modelHandle);
	~Jump();

	void Initialize(const int modelHandle,const int changeNum, Player& player)override;
	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)override;

	bool MotionUpdate(PlayerData& playerData)override;
	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«
private:
	static constexpr float kPlayAnimSpeed = 0.9f;	    // ˆÚ“®‘¬“x

	bool isPush;		//ƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚½‚©
	bool isRun;
	bool isFirstJump;
	bool isSecondJump;
};

