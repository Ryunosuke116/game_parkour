#pragma once
#include "PlayerStateBase.h"
#include "PlayerData.h"

class Player;

class BracedHangToCrouch : public PlayerStateBase
{
public:
	BracedHangToCrouch(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimState);
	~BracedHangToCrouch();

	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)override;
	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	void Enter(PlayerData& playerData) override;		//状態に入ったとき
	void Exit(PlayerData& playerData) override;			//状態を抜けるとき
private:
	static constexpr float playAnimSpeed = 0.4f;	    // アニメーション速度

};

