#pragma once
#include "DxLib.h"
#include "CollisionData.h"
#include <string>

class PlayerCalculation
{
public:
	PlayerCalculation();
	~PlayerCalculation(){}

	void Initialize();

	VECTOR Update(const VECTOR& moveDirection, const float playAnimTime,
		const int nowAnimNumber, const PlayerData& playerData);

	VECTOR Move(const int& nowAnimNumber, const VECTOR& moveDirection,
		const VECTOR& velocity, const PlayerData& playerData);

	VECTOR Jump(const VECTOR& velocity, const int& nowAnimNumber,
		const PlayerData& playerData);

	VECTOR GravityUpdate(const VECTOR& velocity, const PlayerData& playerData);

	VECTOR Roll(const int& nowAnimNumber,
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

	bool GetIsAddJumpPower()const { return isAddJumpPower; }
	bool GetIsWhenClimbingHitGround()const { return isWhenClimbingHitGround; }
	int GetIgnoreGroundTimer()const { return ignoreGroundTimer; }
	float GetNowMoveSpeed() const { return nowMoveSpeed; }
	float GetMaxMoveSpeed() const { return kMaxRunSpeed; }
	float GetWallRunStopTime()const { return wallRunStopTime; }
	float GetWallRunMaxStopTime()const { return kWallRunMaxStopTime; }
	float GetGravityPower() const { return gravityPower; }
	VECTOR GetWallRunGravity()const { return wallRunGravity; }
	NearestResult GetNearestResult() const { return nearestResult; }

	void ChangeTrueIsAddJumpPower() { isAddJumpPower = true; }
	void SetJumpPower() { nowJumpPower = kAddJumpPower; }
	void SetSecondJumpPower() { nowJumpPower = kAddSecondJumpPower; }
	void SetNowGroundRayPoly(const MV1_COLL_RESULT_POLY& set) { nowGroundRayPoly = set; }
	void SetRightHandPos(const VECTOR& set) { rightHandPos = set; }
	void SetLeftHandPos(const VECTOR& set) { leftHandPos = set; }
	void SetNearestResult(const NearestResult& set) { nearestResult = set; }
	void SetWallRunGravity(const VECTOR& set) { wallRunGravity = set; }

private:
	VECTOR CalcProjectionVelocity(const PlayerData& playerData,
		const VECTOR& newVelocity);
	VECTOR AdjustmentVelocity(const PlayerData& playerData,
		const VECTOR& moveDirection);

	float SpeedUp();
	float SpeedDown(const PlayerData& playerData);
	float ChangeRankScore(float mulMoveSpeedValue);

private:
	static constexpr float kMaxRunSpeed = 1.6f;	    // 移動速度
	static constexpr float kMaxDashSpeed = 2.6f;	//ダッシュ速度
	static constexpr float kMaxRollMoveSpeed = 2.5f;	//ロール速度
	static constexpr float kAddJumpPower = 2.0f;		//ジャンプパワー
	static constexpr float kAddSecondJumpPower = 1.5f;		//二段目ジャンプパワー
	static constexpr float kGravity = -0.06f;
	static constexpr float kWallRunMaxStopTime = 10.0f;	
	static constexpr float kMulValueD = 1.0f;
	static constexpr float kMulValueC = 1.05f;
	static constexpr float kMulValueB = 1.1f;
	static constexpr float kMulValueA = 1.2f;
	static constexpr float kMulValueS = 1.3f;
	static constexpr float kMulValueSS = 1.4f;
	static constexpr float kMulValueSSS = 1.45f;

	int ignoreGroundTimer;		//接地判定を無視する時間

	float nowJumpPower;			//現在のジャンプ速度
	float nowMoveSpeed;			//移動速度
	float decelerationSpeed;	//減速速度
	float nowRollMoveSpeed;		//現在のロール速度
	float wallRunStopTime;		//壁に留まる時間
	float gravityPower;			//現在かかっている重力
	float mulMoveSpeedValue;	//現在の移動速度に乗算する値
	
	bool isCalcDeceleration;		//止まるときに一度だけ減速スピードを計算
	bool isStopRunWall;
	bool isAddJumpPower;
	bool isWhenClimbingHitGround;	//登っているときに床と接触しているか

	VECTOR rightHandPos;
	VECTOR leftHandPos;
	VECTOR wallRunGravity;

	std::string nowRankScore;

	NearestResult nearestResult;
	MV1_COLL_RESULT_POLY nowGroundRayPoly;
};

