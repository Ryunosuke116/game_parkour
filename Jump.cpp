#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Jump.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Jump::Jump(int modelHandle) : PlayerStateActionBase(modelHandle)
{
    currentPlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
Jump::~Jump()
{
    MV1DetachAnim(modelHandle, currentAttachIndex);
}