#pragma once
#include "PlayerStateBase.h"

class Player;

class Jump : public PlayerStateBase
{
public:
	Jump(int& modelHandle, AnimState& oldAnimState,
		AnimState& nowAnimState, 
		PlayerData& playerData);
	~Jump();

	void Initialize(int& modelHandle,const int changeNum, Player& player)override;
	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)override;

	bool MotionUpdate(PlayerData& playerData)override;
	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	void Enter(PlayerData& playerData) override;		//ó‘Ô‚É“ü‚Á‚½‚Æ‚«
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«
private:
	static constexpr float playAnimSpeed = 0.9f;	    // ˆÚ“®‘¬“x

	bool isPush;		//ƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚½‚©
	bool isRun;
	bool isJumpFirst;
	bool isJumpSecond;
};

