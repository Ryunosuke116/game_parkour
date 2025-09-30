#pragma once
#include "DxLib.h"
#include"playerState.h"
#include"Calculation.h"

class PlayerCalculation
{
public:

	PlayerCalculation();
	~PlayerCalculation(){}

	VECTOR Update(const VECTOR& moveDirection, const float playAnimTime,
		const int NowAnimNumber, const PlayerData& playerData);

	VECTOR Move(const int& NowAnimNumber, const VECTOR& moveDirection,
		const VECTOR& velocity, const PlayerData& playerData);

	VECTOR Jump(const VECTOR& velocity, const int& NowAnimNumber,
		const PlayerData& playerData);

	VECTOR Gravity(const VECTOR& velocity, const PlayerData& playerData);

	VECTOR Roll(const int& NowAnimNumber,
		const VECTOR& velocity,
		const VECTOR& moveDirection,
		const float playAnimTime,
		const PlayerData& playerData);

	VECTOR HangingAngle(const MV1_COLL_RESULT_POLY& hangingPoly);

	VECTOR HangingPosition();

	VECTOR HangingDirection(const VECTOR& centerPos);

	VECTOR RunWall(const VECTOR& velocity,
		const PlayerData& playerData);

	VECTOR HangToCrouchMove(
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects,
		const std::shared_ptr<Player> player);

	std::pair<bool, VECTOR> GroundCollisionCheckHangToCrouch(
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects,
		const VECTOR& topPos,
		const VECTOR& foot,
		const VECTOR& position);

	void ResetMove();
	void ResetWallRun();

	bool GetIsJumpPower_add()const { return isAddJumpPower; }
	bool GetIsWhenClimbingHitGround()const { return isWhenClimbingHitGround; }
	float GetjumpSpeed_now() const { return nowJumpPower; }
	float GetMoveSpeed_now() const { return nowMoveSpeed; }
	float GetMoveSpeed_max() const { return kMaxRunSpeed; }
	float GetWallRun_stopTime()const { return WallRunStopTime; }
	float GetWallRun_stopTime_max()const { return kWallRunMaxStopTime; }
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
	void SetNowGroundRayPoly(const MV1_COLL_RESULT_POLY& set) { nowGroundRayPoly = set; }
	void SetHangingPoint(const VECTOR& set) { hangingPoint = set; }
	void SetHandPos_right(const VECTOR& set) { handPos_right = set; }
	void SetHandPos_left(const VECTOR& set) { handPos_left = set; }
	void SethitWallNormal(const VECTOR& set) { hitWallNormal = set; }
	void SetNearestResult(const Calculation::NearestResult& set) { nearestResult = set; }
	void SetWallRunGravity(const VECTOR& set) { wallRunGravity = set; }
private:
	static constexpr float kMaxRunSpeed = 1.6f;	    // 移動速度
	static constexpr  float kMaxDashSpeed = 2.6f;	//ダッシュ速度
	static constexpr float kMaxRollMoveSpeed = 2.5f;	//ロール速度
	static constexpr float addJumpPower = 2.0f;		//ジャンプパワー
	static constexpr float addJumpPowe_second = 1.5f;		//二段目ジャンプパワー
	static constexpr float gravity = -0.06f;
	static constexpr float kWallRunMaxStopTime = 10.0f;	

	float nowJumpPower;			//現在のジャンプスピード
	float nowMoveSpeed;
	float decelerationSpeed = 0.0f;		//減速速度
	float nowRollMoveSpeed;		//現在のロールスピード
	float WallRunStopTime;		//壁に留まる時間
	float gravityPower;			//現在かかっている重力
	

	bool isCalcDeceleration;		//止まるときに一度だけ減速スピードを計算
	bool isSlip_after;				//slipした後か
	bool isStopRunWall;
	bool isAddJumpPower;
	bool isWhenClimbingHitGround;	//登っているときに床と接触しているか

	VECTOR hangingPoint;
	VECTOR handPos_right;
	VECTOR handPos_left;
	VECTOR hitWallNormal;
	VECTOR wallRunGravity;

	Calculation::NearestResult nearestResult;
	MV1_COLL_RESULT_POLY hangingPoly;
	MV1_COLL_RESULT_POLY nowGroundRayPoly;
	
private:
	VECTOR CalcProjectionVelocity(const PlayerData& playerData,
		const VECTOR& newVelocity);
	VECTOR AdjustmentVelocity(const PlayerData& playerData,
		const VECTOR& moveDirection);

	float SpeedUp();
	float SpeedDown(const PlayerData& playerData);
};

