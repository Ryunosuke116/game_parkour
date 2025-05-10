#include <iostream>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Falling_Idle.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
<<<<<<< HEAD
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
=======
Falling_Idle::Falling_Idle(int modelHandle) : PlayerStateActionBase(modelHandle)
{
	currentPlayAnimSpeed = playAnimSpeed;
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Falling_Idle::~Falling_Idle()
{
<<<<<<< HEAD
	MV1DetachAnim(modelHandle, nowAnimState.currentAttachIndex);
=======
	MV1DetachAnim(modelHandle, currentAttachIndex);
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c
}

/// <summary>
/// �A�j���[�V�����X�V
/// </summary>
void Falling_Idle::MotionUpdate()
{
	//�Đ����ԍX�V
<<<<<<< HEAD
	nowAnimState.currentPlayTime_anim += nowAnimState.currentPlayAnimSpeed;

	//���Đ����Ԃ𒴂����烊�Z�b�g
	if (nowAnimState.currentPlayTime_anim >= nowAnimState.currentTotalPlayTime_anim)
	{
		nowAnimState.currentPlayTime_anim = static_cast<float>(fmod(nowAnimState.currentPlayTime_anim, nowAnimState.currentTotalPlayTime_anim));
	}

	// �Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(modelHandle, nowAnimState.currentAttachIndex, nowAnimState.currentPlayTime_anim);
=======
	currentPlayTime_anim += currentPlayAnimSpeed;

	//���Đ����Ԃ𒴂����烊�Z�b�g
	if (currentPlayTime_anim >= currentTotalPlayTime_anim)
	{
		currentPlayTime_anim = static_cast<float>(fmod(currentPlayTime_anim, currentTotalPlayTime_anim));
	}

	// �Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(modelHandle, currentAttachIndex, currentPlayTime_anim);
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c
}