#pragma once
#include "PlayerStateBase.h"

class Player;

class Run : public PlayerStateBase
{
public:
	Run(const int modelHandle);
	~Run();

	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)			override;
	
	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)	override;
	VECTOR Move(const VECTOR& cameraDirection, PlayerData& playerData)						override;
	void Enter(const AnimState& oldAnimState, const AnimState& nowAnimState)				override;		//ó‘Ô‚É“ü‚Á‚½‚Æ‚«
	void Exit(PlayerData& playerData)														override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«

	std::pair<VECTOR, PlayerData> NormalUpdate(const VECTOR& cameraDirection, Player& player);
	std::pair<VECTOR, PlayerData> WallRunUpdate(Player& player,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects);

private:
	static constexpr float kPlayAnimSpeed = 0.45f;	    // ˆÚ“®‘¬“x
	static constexpr float DashAnimSpeed = 0.9f;
};

