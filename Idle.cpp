#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Idle.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle"></param>
<<<<<<< HEAD
Idle::Idle(int modelHandle, int& prevAttachIndex, NowAnimState nowAnimState) : 
    PlayerStateActionBase(modelHandle, prevAttachIndex,nowAnimState)
{
    // �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
    this->nowAnimState.currentAttachIndex = MV1AttachAnim(modelHandle, animNum::Idle);

    this->nowAnimState.currentPlayAnimSpeed = playAnimSpeed;
=======
Idle::Idle(int modelHandle) : PlayerStateActionBase(modelHandle)
{
    currentPlayAnimSpeed = playAnimSpeed;
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Idle::~Idle()
{
<<<<<<< HEAD
    MV1DetachAnim(modelHandle, this->nowAnimState.currentAttachIndex);
=======
    MV1DetachAnim(modelHandle, currentAttachIndex);
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c
}