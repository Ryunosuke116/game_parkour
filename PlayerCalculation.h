#pragma once
#include "DxLib.h"
#include"playerState.h"
#include"Calculation.h"

class PlayerCalculation
{
public:

	PlayerCalculation();
	~PlayerCalculation(){}

	VECTOR Update(const VECTOR& moveVec, const VECTOR& moveDirection, const float playTime_anim,
		const int& animNumber_Now, const PlayerStateActionBase::PlayerData& playerData);

	VECTOR Move(const VECTOR& moveVec, const VECTOR& moveDirection, const PlayerStateActionBase::PlayerData& playerData);

	VECTOR Jump(const VECTOR& moveVec, const int& animNumber_Now,
		const PlayerStateActionBase::PlayerData& playerData);

	void Gravity(const VECTOR& moveVec, const PlayerStateActionBase::PlayerData& playerData);

	VECTOR Roll(const VECTOR& moveVec, const VECTOR& moveDirection, 
		float playTime_anim, const PlayerStateActionBase::PlayerData& playerData);

	VECTOR MoveVec(const VECTOR& moveVec, const VECTOR& moveVec_memory, const bool isGround, const bool isRoll);

	VECTOR HangingAngle(const MV1_COLL_RESULT_POLY& hangingPoly);

	VECTOR HangingPosition(const VECTOR& handPos_left, const VECTOR& handPos_right, const VECTOR& nearestPoint);

	VECTOR HangingDirection(const MV1_COLL_RESULT_POLY& hangingPoly, const VECTOR& centerPos);

	void Reset_move();

	float GetjumpSpeed_now() const { return jumpSpeed_now; }
	float GetmoveSpeed_now() const { return moveSpeed_now; }
	float GetmoveSpeed_max() const { return MaxMoveSpeed; }
	Calculation::NearestResult GetNearestResult() { return nearestResult; }
	void SetjumpSpeed_now(const float& jumpSpeed) { jumpSpeed_now = jumpSpeed; }

private:
	static constexpr float MaxMoveSpeed = 1.6f;	    // 移動速度
	static constexpr float rollMoveSpeed_max = 2.5f;	//ロール速度
	static constexpr float addJumpPower = 2.0f;		//ジャンプパワー
	static constexpr float gravity = -0.06f;

	float jumpSpeed_now;			//現在のジャンプスピード
	float moveSpeed_now;
	float rollMoveSpeed_now;		//現在のロールスピード
	float decelerationSpeed;		//減速速度

	bool isCalc_deceleration;
	bool isSlip_after;

	Calculation::NearestResult nearestResult;

};

