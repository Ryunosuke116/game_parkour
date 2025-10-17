#pragma once
#include "PlayerStateBase.h"
#include "PlayerData.h"

class Player;

class BracedHangToCrouch : public PlayerStateBase
{
public:
	BracedHangToCrouch(const int modelHandle);
	~BracedHangToCrouch();

	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)override;
	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	void Exit(PlayerData& playerData) override;			//状態を抜けるとき

private:
	static constexpr float kPlayAnimSpeed = 0.4f;	    // アニメーション速度

};

