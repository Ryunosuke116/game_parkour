#pragma once
class PlayerStateActionBase
{
public:
	PlayerStateActionBase(int modelHandle);
	~PlayerStateActionBase() {};

	virtual void MotionUpdate();

protected:

	static constexpr float	AnimBlendSpeed = 0.1f;		// �A�j���[�V�����̃u�����h���ω����x

	int modelHandle;			//���f���n���h��
	float animBlendRate;

	int prevAttachIndex;			//�O�̃A�j���[�V�������
	float prevPlayTime_anim;		//�O��̍Đ�����
	float prevTotalPlayTime_anim;	//�O��̑��Đ�����
	float prevPlayAnimSpeed;		//�O��̃A�j���[�V�����X�s�[�h

	int currentAttachIndex;				//���݂̃A�j���[�V�������
	float currentPlayTime_anim;			//���݂̍Đ�����
	float currentTotalPlayTime_anim;	//���݂̑��Đ�����
	float currentPlayAnimSpeed;			//�O��̃A�j���[�V�����X�s�[�h


	//�ړ�
	//VECTOR moveVec;

	enum animNum : int
	{
		Braced_Hang_To_Crouch,		//���Ⴊ��łԂ炳����
		Falling_To_Roll,			//���n���ē]����
		Hangring_Idle,				//�Ԃ炳����
		Hard_Landing,				//���n����
		Idle,						//�Î~��
		Jump,						//�W�����v
		Quick_Roll,					//�]����
		Run,						//����
		Run_Jump,					//����Ȃ���W�����v
		Run_To_Stop,				//�~�܂�
		Running_Forward_Flip,		//����Ȃ����]�W�����v
		Falling_Idle				//�����Ă���Ƃ�
	};
};

