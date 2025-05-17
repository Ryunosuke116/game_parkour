#pragma once
#include "PlayerStateActionBase.h"

class Player : public BaseChara
{
private:
	static constexpr float modelScale = 0.06f;
	static constexpr float MoveSpeed = 0.4f;	    // �ړ����x
	static constexpr float attackAnimSpeed = 1.0f;	//�U�����x
	static constexpr float rollAnimSpeed = 1.5f;	//�U�����x
	static constexpr float angleSpeed = 0.3f;
	static constexpr float addJumpPower = 1.9f;		//�W�����v�p���[
	static constexpr float gravity = 0.1f;


	float currentJumpSpeed;			//���݂̃W�����v�X�s�[�h

	bool isPush;					//�{�^������������
	bool isChageState;				//�A�j���[�V������ύX���邩

	enum animNum : int
	{
		braced_Hang_To_Crouch,		//���Ⴊ��łԂ炳����
		falling_To_Roll,			//���n���ē]����
		hangring_Idle,				//�Ԃ炳����
		hard_Landing,				//���n����
		idle,						//�Î~��
		jump,						//�W�����v
		quick_Roll,					//�]����
		run,						//����
		run_Jump,					//����Ȃ���W�����v
		run_To_Stop,				//�~�܂�
		running_Forward_Flip,		//����Ȃ����]�W�����v
		falling_Idle				//�����Ă���Ƃ�
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

	//���N���X
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

