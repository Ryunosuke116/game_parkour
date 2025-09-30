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
		const int animNumber_Now, const PlayerData& playerData);

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

	VECTOR HangingDirection(const VECTOR& centerPos);

	VECTOR Run_Wall(const VECTOR& moveVec,
		const PlayerData& playerData);

	VECTOR HangToCrouchMove(
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects,
		const std::weak_ptr<Player> player);

	std::pair<bool, VECTOR> GroundCollisionCheckHangToCrouch(
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects,
		const VECTOR& topPos,
		const VECTOR& foot,
		const VECTOR& position);

	void Reset_move();
	void Reset_run_wall();
	void ResetMoveVec_old() { moveVec_old = VGet(0.0f, 0.0f, 0.0f); }

	bool GetIsJumpPower_add()const { return isAddJumpPower; }
	bool GetIsWhenClimbingHitGround()const { return isWhenClimbingHitGround; }
	float GetjumpSpeed_now() const { return nowJumpPower; }
	float GetMoveSpeed_now() const { return moveSpeed_now; }
	float GetMoveSpeed_max() const { return MaxRunSpeed; }
	float GetWallRun_stopTime()const { return WallRunStopTime; }
	float GetWallRun_stopTime_max()const { return wallRun_stopTime_max; }
	VECTOR GetHangingPoint()const { return hangingPoint; }
	VECTOR GethitWallNormal() const { return hitWallNormal; }
	VECTOR GetWallRunGravity()const { return wallRunGravity; }
	MV1_COLL_RESULT_POLY GetHangingPoly() { return hangingPoly; }
	Calculation::NearestResult GetNearestResult() { return nearestResult; }

	void ChangeIsJumpPower_add_ture() { isAddJumpPower = true; }
	void SetJumpPower() { nowJumpPower = addJumpPower; }
	void SetJumpPower_second() { nowJumpPower = addJumpPowe_second; }
	void SetjumpSpeed_now(const float& jumpSpeed) { nowJumpPower = jumpSpeed; }
	void SetHangingPoly(const MV1_COLL_RESULT_POLY& set) { hangingPoly = set; }
	void SetHangingPoint(const VECTOR& set) { hangingPoint = set; }
	void SetHandPos_right(const VECTOR& set) { handPos_right = set; }
	void SetHandPos_left(const VECTOR& set) { handPos_left = set; }
	void SethitWallNormal(const VECTOR& set) { hitWallNormal = set; }
	void SetNearestResult(const Calculation::NearestResult& set) { nearestResult = set; }
	void SetWallRunGravity(const VECTOR& set) { wallRunGravity = set; }
private:
	static constexpr float MaxRunSpeed = 1.6f;	    // 移動速度
	static constexpr  float MaxDashSpeed = 2.6f;	//ダッシュ速度
	static constexpr float rollMoveSpeed_max = 2.5f;	//ロール速度
	static constexpr float addJumpPower = 2.0f;		//ジャンプパワー
	static constexpr float addJumpPowe_second = 1.5f;		//二段目ジャンプパワー
	static constexpr float gravity = -0.06f;
	static constexpr float wallRun_stopTime_max = 10.0f;	

	float nowJumpPower;			//現在のジャンプスピード
	float moveSpeed_now;
	float rollMoveSpeed_now;		//現在のロールスピード
	float decelerationSpeed;		//減速速度
	float WallRunStopTime;		//壁に留まる時間
	float velocity_gravity;			//現在かかっている重力
	

	bool isCalcDeceleration;		//止まるときに一度だけ減速スピードを計算
	bool isSlip_after;				//slipした後か
	bool isStopRunWall;
	bool isAddJumpPower;
	bool isWhenClimbingHitGround;	//登っているときに床と接触しているか

	VECTOR moveVec_old;
	VECTOR hangingPoint;
	VECTOR handPos_right;
	VECTOR handPos_left;
	VECTOR hitWallNormal;
	VECTOR wallRunGravity;

	Calculation::NearestResult nearestResult;
	MV1_COLL_RESULT_POLY hangingPoly;

};

