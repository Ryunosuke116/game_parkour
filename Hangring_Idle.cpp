#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Hangring_Idle.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldAnimState"></param>
/// <param name="nowAnimState"></param>
/// <param name="playerData"></param>
Hangring_Idle::Hangring_Idle(int& modelHandle,
    AnimState& oldAnimState, AnimState& nowAnimState, PlayerData& playerData) :
    PlayerStateActionBase(modelHandle, oldAnimState, nowAnimState)
{
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::hangring_Idle);

    this->nowAnimState.PlayTime_anim = 0.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;

}

/// <summary>
/// デストラクタ
/// </summary>
Hangring_Idle::~Hangring_Idle()
{

}

//bool Hangring_Idle::MotionUpdate(PlayerData& playerData)
//{
//    //崖を掴む
//    //↑キーを入力したら掴んでいる床に立つ
//    return false;
//}