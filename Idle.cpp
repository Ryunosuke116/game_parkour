#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Idle.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Idle::Idle(int& modelHandle,
    OldAnimState& oldAnimState, NowAnimState& nowAnimState) :
    PlayerStateActionBase(modelHandle, oldAnimState,nowAnimState)
{
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::Idle);

    this->nowAnimState.PlayTime_anim = 0.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
Idle::~Idle()
{
  //  MV1DetachAnim(modelHandle, this->nowAnimState.AttachIndex);
}