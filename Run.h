#pragma once
#include "PlayerStateBase.h"

class Player;

class Run : public PlayerStateBase
{
public:
	Run(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimState, std::shared_ptr<ISoundPlayer> sound);
	~Run();

	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)override;

	bool MotionUpdate(PlayerData& playerData)override;
	
	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	void Enter(PlayerData& playerData) override;		//ó‘Ô‚É“ü‚Á‚½‚Æ‚«
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«
	VECTOR Move(const VECTOR& cameraDirection, PlayerData& playerData)override;

	std::pair<VECTOR, PlayerData> Update_normal(const VECTOR& cameraDirection, Player& player);
	std::pair<VECTOR, PlayerData> Update_wallRun(Player& player,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects);

private:
	static constexpr float playAnimSpeed = 0.45f;	    // ˆÚ“®‘¬“x
	float degree_difference;
	float stopTime;				//‰½f~‚Ü‚Á‚Ä‚¢‚é‚©
	float angle;
	float playerMoveSpeed;
	float playerMoveSpeed_max;
	float degree_new;
	int animationNum_now;
};

