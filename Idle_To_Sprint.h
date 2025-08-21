#pragma once
#include "PlayerStateActionBase.h"

class Idle_To_Sprint : public PlayerStateActionBase
{
public:
	Idle_To_Sprint(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimState);
	~Idle_To_Sprint();

	void Initialize(int& modelHandle, Player& player)override;

	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::shared_ptr<BaseObject>>& collisionObjects, Player& player)override;

	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;

	bool MotionUpdate(PlayerData& playerData)override;
	void Enter(PlayerData& playerData) override;		//ó‘Ô‚É“ü‚Á‚½‚Æ‚«
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«

private:
	static constexpr float playAnimSpeed = 0.6f;	    // ˆÚ“®‘¬“x
};

