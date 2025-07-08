#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Hanging_Idle.h"
#include "AnimTime.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldAnimState"></param>
/// <param name="nowAnimState"></param>
/// <param name="playerData"></param>
Hanging_Idle::Hanging_Idle(int& modelHandle,
    AnimState& oldAnimState, AnimState& nowAnimState, PlayerData& playerData) :
    PlayerStateActionBase(modelHandle, oldAnimState, nowAnimState)
{
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::hanging_Idle);

    this->nowAnimState.PlayTime_anim = 0.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
    this->nowAnimState.TotalPlayTime_anim = MV1GetAttachAnimTotalTime(modelHandle, this->nowAnimState.AttachIndex);
}

/// <summary>
/// デストラクタ
/// </summary>
Hanging_Idle::~Hanging_Idle()
{

}

//bool Hanging_Idle::MotionUpdate(PlayerData& playerData)
//{
//    //崖を掴む
//    //↑キーを入力したら掴んでいる床に立つ
//    return false;
//}