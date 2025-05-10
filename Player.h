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

	bool isMove;					//動いているか
	bool isJump;					//ジャンプしたか
	bool isJump_second;				//二段ジャンプしたか
	bool isPush;					//ボタンを押したか

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

	//他クラス
	std::shared_ptr<PlayerStateActionBase> nowState = NULL;
<<<<<<< HEAD
	PlayerStateActionBase::NowAnimState nowAnimState;
=======
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c

public:
	Player();
	~Player();

	void Initialize();
	void Update(const Input& input, const VECTOR& cameraDirection);
	void Draw();
	void Move(const Input& input, VECTOR& moveVec, const VECTOR& cameraDirection);
	void JumpMove(const Input& input);
	void JumpCalclation();
	void ChangeMotion(const int& motionNum, const float playAnimSpeed)override;
	void MotionUpdate() override;
	void ChangeState();

	struct PadInput
	{
		bool isUp(const Input& input);
		bool isDown(const Input& input);
		bool isRight(const Input& input);
		bool isLeft(const Input& input);
		bool isJump(const Input& input);
	};

	PadInput padInput;
<<<<<<< HEAD

	void SetNowAnimState(PlayerStateActionBase::NowAnimState animState);

=======
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c
};

