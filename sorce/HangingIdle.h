#pragma once
#include "PlayerStateBase.h"

class Player;

class HangingIdle : public PlayerStateBase
{
public:
	HangingIdle(const int modelHandle);

	~HangingIdle();

	void Initialize(const int modelHandle,const int changeNum, Player& player)override;
	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)override;
	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	void Exit(PlayerData& playerData) override;			//状態を抜けるとき


private:
	static constexpr float kPlayAnimSpeed = 0.4f;	    // アニメーション速度
};

