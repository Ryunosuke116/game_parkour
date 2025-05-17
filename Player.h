#pragma once
#include "PlayerStateActionBase.h"

class Player : public BaseChara
{
private:
	static constexpr float modelScale = 0.06f;
	static constexpr float MoveSpeed = 0.4f;	    // 移動速度
	static constexpr float attackAnimSpeed = 1.0f;	//攻撃速度
	static constexpr float rollAnimSpeed = 1.5f;	//攻撃速度
	static constexpr float angleSpeed = 0.3f;
	static constexpr float addJumpPower = 1.9f;		//ジャンプパワー
	static constexpr float gravity = 0.1f;


	float currentJumpSpeed;			//現在のジャンプスピード

	bool isPush;					//ボタンを押したか
	bool isChageState;				//アニメーションを変更するか

	enum animNum : int
	{
		braced_Hang_To_Crouch,		//しゃがんでぶらさがる
		falling_To_Roll,			//着地して転がる
		hangring_Idle,				//ぶらさがる
		hard_Landing,				//着地する
		idle,						//静止時
		jump,						//ジャンプ
		quick_Roll,					//転がる
		run,						//走る
		run_Jump,					//走りながらジャンプ
		run_To_Stop,				//止まる
		running_Forward_Flip,		//走りながら回転ジャンプ
		falling_Idle				//落ちているとき
	};

	struct PadInput
	{
		bool isUp(Input& input);
		bool isDown(Input& input);
		bool isRight(Input& input);
		bool isLeft(Input& input);
		bool isJump(Input& input);
		bool isRoll(Input& input);
	};

	PadInput padInput;

	//他クラス
	std::shared_ptr<PlayerStateActionBase> nowState = NULL;
	PlayerStateActionBase::OldAnimState oldAnimState;
	PlayerStateActionBase::NowAnimState nowAnimState;
	std::shared_ptr<Input> input = NULL;
	PlayerStateActionBase::PlayerData playerData;

public:
	Player();
	~Player();

	void Initialize();
	void Update(const VECTOR& cameraDirection);
	void Draw();
	void Move(VECTOR& moveVec, const VECTOR& cameraDirection);
	void JumpMove();
	void JumpCalclation(float playTime_anim);
	void RollMove();
	//void ChangeMotion(const int& motionNum, const float playAnimSpeed)override;
	//void MotionUpdate();
	void ChangeState();


	void SetOldAnimState(PlayerStateActionBase::OldAnimState animState);
	void SetNowAnimState(PlayerStateActionBase::NowAnimState animState);
};

