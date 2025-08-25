#pragma once
#include "PlayerStateBase.h"

class Player;

class Hanging_Idle : public PlayerStateBase
{
public:
	Hanging_Idle(int& modelHandle,
		AnimState& oldAnimState, 
		AnimState& nowAnimStatel, 
		std::shared_ptr<ISoundPlayer> sound);

	~Hanging_Idle();

	void Initialize(int& modelHandle,const int changeNum, Player& player)override;
	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)override;
	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	void Enter(PlayerData& playerData) override;		//状態に入ったとき
	void Exit(PlayerData& playerData) override;			//状態を抜けるとき


private:
	static constexpr float playAnimSpeed = 0.4f;	    // アニメーション速度
};

