#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Jump.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Jump::Jump(int modelHandle, int& prevAttachIndex, NowAnimState nowAnimState) :
    PlayerStateActionBase(modelHandle, prevAttachIndex,nowAnimState)
{
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    this->nowAnimState.currentAttachIndex = MV1AttachAnim(modelHandle, animNum::Jump);

    this->nowAnimState.currentPlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
Jump::~Jump()
{
    MV1DetachAnim(modelHandle, this->nowAnimState.currentAttachIndex);
}