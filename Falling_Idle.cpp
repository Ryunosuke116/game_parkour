#include <iostream>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Falling_Idle.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Falling_Idle::Falling_Idle(int modelHandle) : PlayerStateActionBase(modelHandle)
{
	currentPlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Falling_Idle::~Falling_Idle()
{
	MV1DetachAnim(modelHandle, currentAttachIndex);
}

/// <summary>
/// �A�j���[�V�����X�V
/// </summary>
void Falling_Idle::MotionUpdate()
{
	//�Đ����ԍX�V
	currentPlayTime_anim += currentPlayAnimSpeed;

	//���Đ����Ԃ𒴂����烊�Z�b�g
	if (currentPlayTime_anim >= currentTotalPlayTime_anim)
	{
		currentPlayTime_anim = static_cast<float>(fmod(currentPlayTime_anim, currentTotalPlayTime_anim));
	}

	// �Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(modelHandle, currentAttachIndex, currentPlayTime_anim);
}