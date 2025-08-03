#pragma once
#include "DxLib.h"
#include"playerState.h"
#include"Calculation.h"

class PlayerCalculation
{
public:

	PlayerCalculation();
	~PlayerCalculation(){}

	VECTOR Update(const VECTOR& moveDirection, const float playTime_anim,
		const int& animNumber_Now, const PlayerData& playerData);

	VECTOR Move(const int& animNumber_Now, const VECTOR& moveDirection,
		const VECTOR& moveVec, const PlayerData& playerData);

	VECTOR Jump(const VECTOR& moveVec, const int& animNumber_Now,
		const PlayerData& playerData);

	VECTOR Gravity(const VECTOR& moveVec, const PlayerData& playerData);

	VECTOR Roll(const int& animNumber_Now,
		const VECTOR& moveVec, const VECTOR& moveDirection,
		float playTime_anim, const PlayerData& playerData);

	VECTOR HangingAngle(const MV1_COLL_RESULT_POLY& hangingPoly);

	VECTOR HangingPosition();

	VECTOR HangingDirection(const MV1_COLL_RESULT_POLY& hangingPoly, const VECTOR& centerPos);

	VECTOR Run_Wall(const VECTOR& moveVec,
		const PlayerData& playerData);

	void Reset_move();
	void Reset_run_wall();

	bool GetIsJumpPower_add()const { return isJumpPower_add; }
	float GetjumpSpeed_now() const { return jumpPower_now; }
	float GetMoveSpeed_now() const { return moveSpeed_now; }
	float GetMoveSpeed_max() const { return MaxMoveSpeed; }
	float GetWallRun_stopTime()const { return wallRun_stopTime; }
	float GetWallRun_stopTime_max()const { return wallRun_stopTime_max; }
	VECTOR GetHangingPoint()const { return hangingPoint; }
	VECTOR GetHitWall_normal() const { return hitWall_normal; }
	MV1_COLL_RESULT_POLY GetHangingPoly() { return hangingPoly; }
	Calculation::NearestResult GetNearestResult() { return nearestResult; }

	void ChangeIsJumpPower_add_ture() { isJumpPower_add = true; }
	void SetJumpPower() { jumpPower_now = addJumpPower; }
	void SetjumpSpeed_now(const float& jumpSpeed) { jumpPower_now = jumpSpeed; }
	void SetHangingPoly(const MV1_COLL_RESULT_POLY& set) { hangingPoly = set; }
	void SetHangingPoint(const VECTOR& set) { hangingPoint = set; }
	void SetHandPos_right(const VECTOR& set) { handPos_right = set; }
	void SetHandPos_left(const VECTOR& set) { handPos_left = set; }
	void SetHitWall_normal(const VECTOR& set) { hitWall_normal = set; }
private:
	static constexpr float MaxMoveSpeed = 1.6f;	    // 移動速度
	static constexpr float rollMoveSpeed_max = 2.5f;	//ロール速度
	static constexpr float addJumpPower = 2.0f;		//ジャンプパワー
	static constexpr float gravity = -0.06f;
	static constexpr float wallRun_stopTime_max = 10.0f;	

	float jumpPower_now;			//現在のジャンプスピード
	float moveSpeed_now;
	float rollMoveSpeed_now;		//現在のロールスピード
	float decelerationSpeed;		//減速速度
	float wallRun_stopTime;		//壁に留まる時間
	float velocity_gravity;			//現在かかっている重力
	

	bool isCalc_deceleration;		//止まるときに一度だけ減速スピードを計算
	bool isSlip_after;				//slipした後か
	bool isRun_Wall_Stop;
	bool isJumpPower_add;

	VECTOR moveVec_old;
	VECTOR hangingPoint;
	VECTOR handPos_right;
	VECTOR handPos_left;
	VECTOR hitWall_normal;

	Calculation::NearestResult nearestResult;
	MV1_COLL_RESULT_POLY hangingPoly;

};

