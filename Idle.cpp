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
    OldAnimState& oldAnimState, NowAnimState& nowAnimState, PlayerData& playerData) :
    PlayerStateActionBase(modelHandle, oldAnimState,nowAnimState)
{
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::idle);

    this->nowAnimState.PlayTime_anim = 0.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
    playerData.isSprint = false;
}

/// <summary>
/// デストラクタ
/// </summary>
Idle::~Idle()
{
  //  MV1DetachAnim(modelHandle, this->nowAnimState.AttachIndex);
}