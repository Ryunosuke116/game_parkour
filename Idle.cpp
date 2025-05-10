#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Idle.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Idle::Idle(int modelHandle, int& prevAttachIndex, NowAnimState nowAnimState) : 
    PlayerStateActionBase(modelHandle, prevAttachIndex,nowAnimState)
{
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    this->nowAnimState.currentAttachIndex = MV1AttachAnim(modelHandle, animNum::Idle);

    this->nowAnimState.currentPlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
Idle::~Idle()
{
    MV1DetachAnim(modelHandle, this->nowAnimState.currentAttachIndex);
}