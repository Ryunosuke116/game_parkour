#pragma once
#include <memory>

class Input;

class PlayerStateActionBase
{
public:

	struct PlayerData
	{
		bool isMove;					//�����Ă��邩
		bool isJump;					//�W�����v������
		bool isJump_second;				//��i�W�����v������
		bool isJump_PlayAnim;			//�W�����v�̃A�j�����Đ����邩
		bool isRoll;					//�]���邩
		bool isRoll_PlayAnim;			//���[���A�N�V�����̃A�j�����Đ����邩
		bool isGround;					//�ڒn���Ă��邩
		bool isSprint;					//����o����
		bool isStopRun;					//����I�������
		bool isJumpAll;					//�S�ẴW�����v������������
	};

	struct OldAnimState
	{
		int AttachIndex;			//�O�̃A�j���[�V�������
		float PlayTime_anim;		//�O��̍Đ�����
		float TotalPlayTime_anim;	//�O��̑��Đ�����
		float PlayAnimSpeed;		//�O��̃A�j���[�V�����X�s�[�h
	};

	struct NowAnimState
	{
		int AttachIndex;				//���݂̃A�j���[�V�������
		float PlayTime_anim;			//���݂̍Đ�����
		float TotalPlayTime_anim;	//���݂̑��Đ�����
		float PlayAnimSpeed;			//�O��̃A�j���[�V�����X�s�[�h
	};

	PlayerStateActionBase(int& modelHandle,
		OldAnimState& oldAnimState, NowAnimState& nowAnimState);
	~PlayerStateActionBase() {};

	virtual bool MotionUpdate(PlayerData& playerData);
	void SetOldAnimState();
	void ResetOldAnimState();
	void ResetNowAnimState();
	void Draw();


	//////////////////////////////////////////////
	// �Q�b�^�[
	//////////////////////////////////////////////
	//int GetPrevAttachIndex() { return oldAnimState.AttachIndex; }
	const OldAnimState GetOldAnimState() const { return oldAnimState; }
	const NowAnimState GetNowAnimState() const { return nowAnimState; }

protected:


	static constexpr float	AnimBlendSpeed = 0.1f;		// �A�j���[�V�����̃u�����h���ω����x
	

	int modelHandle;			//���f���n���h��
	float animBlendRate;

	OldAnimState oldAnimState;
	NowAnimState nowAnimState;
	PlayerData  playerData;

	//�ړ�
	//VECTOR moveVec;

	enum animNum : int
	{
		braced_Hang_To_Crouch,		//���Ⴊ��łԂ炳����
		falling_Idle,				//�����Ă���Ƃ�
		falling_To_Roll,			//���n���ē]����
		hangring_Idle,				//�Ԃ炳����
		hard_Landing,				//���n����
		idle,						//�Î~��
		idle_To_Sprint,				//����o��
		jump,						//�W�����v
		jump_Over,					//���[���W�����v
		quick_Roll,					//�]����
		run,						//����
		run_Jump,					//����Ȃ���W�����v
		run_To_Stop,				//�~�܂�
		running_Forward_Flip		//����Ȃ����]�W�����v
	};

	std::shared_ptr<Input> input = NULL;

};

