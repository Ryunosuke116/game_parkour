#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Jump.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle"></param>
Jump::Jump(int modelHandle) : PlayerStateActionBase(modelHandle)
{
    currentPlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Jump::~Jump()
{
    MV1DetachAnim(modelHandle, currentAttachIndex);
}