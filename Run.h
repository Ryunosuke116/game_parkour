#pragma once
#include "PlayerStateActionBase.h"

class Player;

class Run : public PlayerStateActionBase
{
public:
	Run(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimState);
	~Run();

	virtual void Initialize(int& modelHandle, Player& player)override;

	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::shared_ptr<BaseObject>>& collisionObjects, Player& player)override;

	bool MotionUpdate(PlayerData& playerData)override;
	
	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	void Enter(PlayerData& playerData) override;		//ó‘Ô‚É“ü‚Á‚½‚Æ‚«
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«
	VECTOR Move(const VECTOR& cameraDirection, PlayerData& playerData)override;

	std::pair<VECTOR, PlayerData> Update_normal(const VECTOR& cameraDirection, Player& player);
	std::pair<VECTOR, PlayerData> Update_wallRun(Player& player,
		const std::vector<std::shared_ptr<BaseObject>>& collisionObjects);

private:
	static constexpr float playAnimSpeed = 0.5f;	    // ˆÚ“®‘¬“x
	float degree_difference;
	float stopTime;				//‰½f~‚Ü‚Á‚Ä‚¢‚é‚©
	float angle;
	float playerMoveSpeed;
	float playerMoveSpeed_max;
	float degree_new;
	int animationNum_now;
};

