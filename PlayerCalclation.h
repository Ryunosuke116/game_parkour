#pragma once
#include "DxLib.h"
#include"playerState.h"

class PlayerCalclation
{
public:

	PlayerCalclation();
	~PlayerCalclation(){}

	VECTOR Update(const VECTOR& moveVec, const VECTOR& moveDirection, const float playTime_anim,
		const int& animNumber_Now, const PlayerStateActionBase::PlayerData& playerData);
	VECTOR Move(const VECTOR& moveVec, const VECTOR& moveDirection, const PlayerStateActionBase::PlayerData& playerData);
	VECTOR Jump(const VECTOR& moveVec, const int& animNumber_Now,
		const PlayerStateActionBase::PlayerData& playerData);
	void Gravity(const VECTOR& moveVec, const PlayerStateActionBase::PlayerData& playerData);
	VECTOR Roll(const VECTOR& moveVec, const VECTOR& moveDirection, 
		float playTime_anim, const PlayerStateActionBase::PlayerData& playerData);
	VECTOR MoveVec(const VECTOR& moveVec, const VECTOR& moveVec_memory, const bool isGround, const bool isRoll);
	VECTOR HangringAngle(const MV1_COLL_RESULT_POLY& hangringPoly);
	VECTOR HangringPosition(const VECTOR& handPos_left, const VECTOR& handPos_right, const VECTOR& nearestPoint);

	float GetCurrentJumpSpeed() { return currentJumpSpeed; }
	float GetNowMoveSpeed() { return nowMoveSpeed; }

	void SetCurrentJumpSpeed(const float& jumpSpeed) { currentJumpSpeed = jumpSpeed; }

private:
	static constexpr float MaxMoveSpeed = 1.6f;	    // 移動速度
	static constexpr float rollMoveSpeed_max = 2.5f;	//ロール速度
	static constexpr float addJumpPower = 2.0f;		//ジャンプパワー
	static constexpr float gravity = -0.06f;

	float currentJumpSpeed;			//現在のジャンプスピード
	float nowMoveSpeed;
	float rollMoveSpeed_now;		//現在のロールスピード

};

