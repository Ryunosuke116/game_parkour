#pragma once
#include "PlayerStateBase.h"

class Player;

class Run : public PlayerStateBase
{
public:
	Run(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimState);
	~Run();

	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)override;

	bool MotionUpdate(PlayerData& playerData)override;
	
	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	VECTOR Move(const VECTOR& cameraDirection, PlayerData& playerData)override;
	void Enter(PlayerData& playerData) override;		//ó‘Ô‚É“ü‚Á‚½‚Æ‚«
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«

	void DashMove(PlayerData& playerData);
	void ObstacleCheck(
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects,
		const VECTOR& moveDirection,
		const VECTOR& playerPosition,
		const float radius);

	std::pair<VECTOR, PlayerData> Update_normal(const VECTOR& cameraDirection, Player& player);
	std::pair<VECTOR, PlayerData> Update_wallRun(Player& player,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects);

private:
	static constexpr float playAnimSpeed = 0.45f;	    // ˆÚ“®‘¬“x
	static constexpr float DashAnimSpeed = 0.9f;
	float degree_difference;
	float stopTime;				//‰½f~‚Ü‚Á‚Ä‚¢‚é‚©
	float angle;
	float playerMoveSpeed;
	float playerMoveSpeed_max;
	float degree_new;
	int animationNum_now;
};

