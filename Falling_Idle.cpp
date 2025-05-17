#include <iostream>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Falling_Idle.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Falling_Idle::Falling_Idle(int& modelHandle,
	OldAnimState& oldAnimState, NowAnimState& nowAnimState) :
	PlayerStateActionBase(modelHandle, oldAnimState,nowAnimState)
{
	this->nowAnimState.PlayAnimSpeed = playAnimSpeed;

	// �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
	this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::Falling_Idle);

	this->nowAnimState.PlayTime_anim = 0.0f;
	this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
	animBlendRate = 1.0f;

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Falling_Idle::~Falling_Idle()
{
	//MV1DetachAnim(modelHandle, nowAnimState.AttachIndex);
}

/// <summary>
/// �A�j���[�V�����X�V
/// </summary>
bool Falling_Idle::MotionUpdate(PlayerData& playerData)
{
	// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
	float totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.AttachIndex);

	//�Đ����ԍX�V
	nowAnimState.PlayTime_anim += nowAnimState.PlayAnimSpeed;

	//���Đ����Ԃ𒴂����烊�Z�b�g
	if (nowAnimState.PlayTime_anim >= totalTime_anim)
	{
		nowAnimState.PlayTime_anim = static_cast<float>(fmod(nowAnimState.PlayTime_anim, totalTime_anim));
	}

	// �Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(modelHandle, nowAnimState.AttachIndex, nowAnimState.PlayTime_anim);

	//�A�j���[�V�����̃��f���ɑ΂��锽�f�����Z�b�g
	MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.AttachIndex, animBlendRate);

	return false;
}