#pragma once
#include "PlayerStateBase.h"

class Victory : public PlayerStateBase
{
public:
	Victory(int& modelHandle, AnimState& oldAnimState,
		AnimState& nowAnimState,
		PlayerData& playerData);
	~Victory();

	void Initialize(int& modelHandle, const int changeNum, Player& player)override;
	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)override;

	bool MotionUpdate(PlayerData& playerData)override;
	void Enter(PlayerData& playerData) override;		//ó‘Ô‚É“ü‚Á‚½‚Æ‚«
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«

private:
	static constexpr float playAnimSpeed = 0.9f;	    // ˆÚ“®‘¬“x
};

