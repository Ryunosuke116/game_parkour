#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Idle.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle"></param>
Idle::Idle(int modelHandle, int& prevAttachIndex, NowAnimState nowAnimState) : 
    PlayerStateActionBase(modelHandle, prevAttachIndex,nowAnimState)
{
    // �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
    this->nowAnimState.currentAttachIndex = MV1AttachAnim(modelHandle, animNum::Idle);

    this->nowAnimState.currentPlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Idle::~Idle()
{
    MV1DetachAnim(modelHandle, this->nowAnimState.currentAttachIndex);
}