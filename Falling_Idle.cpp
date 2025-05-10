#include <iostream>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Falling_Idle.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Falling_Idle::Falling_Idle(int modelHandle, int& prevAttachIndex, NowAnimState nowAnimState) :
	PlayerStateActionBase(modelHandle, prevAttachIndex, nowAnimState)
{
	this->nowAnimState.currentPlayAnimSpeed = playAnimSpeed;

	//�Â���񂪂���ꍇ�͍폜
	if (prevAttachIndex != -1)
	{
		MV1DetachAnim(modelHandle, prevAttachIndex);
		prevAttachIndex = -1;
	}

	//���܂܂ŏ���prev�ɕۑ�
	prevAttachIndex = this->nowAnimState.currentAttachIndex;
	prevPlayTime_anim = this->nowAnimState.currentPlayTime_anim;
	prevPlayAnimSpeed = this->nowAnimState.currentPlayAnimSpeed;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Falling_Idle::~Falling_Idle()
{
	MV1DetachAnim(modelHandle, nowAnimState.currentAttachIndex);
}

/// <summary>
/// �A�j���[�V�����X�V
/// </summary>
void Falling_Idle::MotionUpdate()
{
	//�Đ����ԍX�V
	nowAnimState.currentPlayTime_anim += nowAnimState.currentPlayAnimSpeed;

	//���Đ����Ԃ𒴂����烊�Z�b�g
	if (nowAnimState.currentPlayTime_anim >= nowAnimState.currentTotalPlayTime_anim)
	{
		nowAnimState.currentPlayTime_anim = static_cast<float>(fmod(nowAnimState.currentPlayTime_anim, nowAnimState.currentTotalPlayTime_anim));
	}

	// �Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(modelHandle, nowAnimState.currentAttachIndex, nowAnimState.currentPlayTime_anim);
}