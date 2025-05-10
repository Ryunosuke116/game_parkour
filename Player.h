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

	bool isMove;					//�����Ă��邩
	bool isJump;					//�W�����v������
	bool isJump_second;				//��i�W�����v������
	bool isPush;					//�{�^������������

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

	//���N���X
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

