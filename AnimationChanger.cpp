#include "common.h"
#include "AnimTime.h"
#include "playerState.h"
#include "PadInput.h"
#include "AnimationChanger.h"
#include "Player.h"

AnimationChanger::AnimationChanger()
{

}

AnimationChanger::~AnimationChanger()
{

}

void AnimationChanger::Initialize(const int& num, int& modelHandle,
    std::shared_ptr<PlayerStateActionBase>& nowState,
    PlayerData& playerData,Player& player)
{
    animNumber_Now = num;
    //newStateを生成
    nowState = std::make_shared<Idle>(modelHandle, oldAnimState,
        nowAnimState, playerData);

    nowState->SetAnimNumber_old(animNumber_Now);
    animNumber_Now = animNum::idle;

    nowState->Initialize(modelHandle, player);
    nowState->Enter(playerData);
}

/// <summary>
/// アニメーション変更
/// </summary>
std::shared_ptr<PlayerStateActionBase> AnimationChanger::ChangeState(int& modelHandle,
    Player& player, PlayerData& playerData,
    std::shared_ptr<PlayerStateActionBase>& nowState)
{
    std::shared_ptr<PlayerStateActionBase> newState = nullptr;

    //立ち
    if (playerData.isIdle && animNumber_Now != animNum::idle)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Idle>(modelHandle, oldAnimState,
            nowAnimState, playerData);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::idle;
    }

    //走る
    if (playerData.isRun && animNumber_Now != animNum::run)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Run>(modelHandle, oldAnimState,
            nowAnimState);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::run;
    }

    //ジャンプ
    if (playerData.isJump &&
        animNumber_Now != animNum::jump &&
        animNumber_Now != animNum::run_Jump)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Jump>(modelHandle, oldAnimState,
            nowAnimState, playerData);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::jump;
    }

    //落下中
    if (playerData.isFalling &&
        animNumber_Now != animNum::falling_Idle)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Falling_Idle>(modelHandle,
            oldAnimState, nowAnimState);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::falling_Idle;
    }

    //転がる
    if (playerData.isRoll && animNumber_Now != animNum::quick_Roll)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Quick_Roll>(modelHandle, 
            oldAnimState, nowAnimState);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::quick_Roll;
    }

    //走り出し
    if (playerData.isSprint && animNumber_Now != animNum::idle_To_Sprint)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Idle_To_Sprint>(modelHandle, 
            oldAnimState, nowAnimState);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::idle_To_Sprint;
    }

    //走り終わり
    if (playerData.isStopRun && animNumber_Now != animNum::run_To_Stop)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Run_To_Stop>(modelHandle,
            oldAnimState, nowAnimState);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::run_To_Stop;
    }

    //崖つかみ
    if (playerData.isHanging && animNumber_Now != animNum::hanging_Idle)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Hanging_Idle>(modelHandle,
            oldAnimState, nowAnimState);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::hanging_Idle;
    }

    //崖のぼり
    if (playerData.isHang_to_Crouch &&
        animNumber_Now != animNum::braced_Hang_To_Crouch)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Braced_Hang_To_Crouch>(modelHandle,
            oldAnimState, nowAnimState);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::braced_Hang_To_Crouch;
    }

    if (newState)
    {
        newState->Initialize(modelHandle, player);
        newState->Enter(playerData);
        return newState;
    }

    return nowState;
}

/// <summary>
/// アニメーション情報をセット
/// </summary>
/// <param name="AnimState"></param>
void AnimationChanger::SetOldAnimState(PlayerStateActionBase::AnimState animState)
{
    oldAnimState.AttachIndex = animState.AttachIndex;
    oldAnimState.PlayAnimSpeed = animState.PlayAnimSpeed;
    oldAnimState.PlayTime_anim = animState.PlayTime_anim;
    oldAnimState.TotalPlayTime_anim = animState.TotalPlayTime_anim;
}

void AnimationChanger::SetNowAnimState(PlayerStateActionBase::AnimState animState)
{
    nowAnimState.AttachIndex = animState.AttachIndex;
    nowAnimState.PlayAnimSpeed = animState.PlayAnimSpeed;
    nowAnimState.PlayTime_anim = animState.PlayTime_anim;
    nowAnimState.TotalPlayTime_anim = animState.TotalPlayTime_anim;
}