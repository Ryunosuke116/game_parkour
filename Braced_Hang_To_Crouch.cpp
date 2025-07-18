#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Braced_Hang_To_Crouch.h"
#include "AnimTime.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldAnimState"></param>
/// <param name="nowAnimState"></param>
/// <param name="playerData"></param>
Braced_Hang_To_Crouch::Braced_Hang_To_Crouch(int& modelHandle,
    AnimState& oldAnimState, AnimState& nowAnimState, PlayerData& playerData) :
    PlayerStateActionBase(modelHandle, oldAnimState, nowAnimState)
{
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::braced_Hang_To_Crouch);

    this->nowAnimState.PlayTime_anim = 0.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
    this->nowAnimState.TotalPlayTime_anim = MV1GetAttachAnimTotalTime(modelHandle, this->nowAnimState.AttachIndex);
}

/// <summary>
/// デストラクタ
/// </summary>
Braced_Hang_To_Crouch::~Braced_Hang_To_Crouch()
{

}
