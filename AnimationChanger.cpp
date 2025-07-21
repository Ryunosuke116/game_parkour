#include "common.h"
#include "AnimTime.h"
#include "playerState.h"
#include "PadInput.h"
#include "AnimationChanger.h"

AnimationChanger::AnimationChanger()
{

}

AnimationChanger::~AnimationChanger()
{

}

/// <summary>
/// アニメーション変更
/// </summary>
void AnimationChanger::ChangeState(int& modelHandle,const bool& isChangeState,
    PlayerStateActionBase::PlayerData& playerData,
    std::shared_ptr<PlayerStateActionBase>& nowState)
{
    //立ち
    if (!playerData.isMove && !playerData.isRun && !playerData.isJump &&
        !playerData.isRoll_PlayAnim &&
        !playerData.isStopRun && !playerData.isHanging &&
        !playerData.isHang_to_Crouch && !playerData.isSlip &&
        animNumber_Now != animNum::idle)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //nowStateの中身を削除
        nowState = nullptr;
        playerData.isIdle = true;
        playerData.isSprint = false;

        //nowStateを更新
        nowState = std::make_shared<Idle>(modelHandle, oldAnimState, 
            nowAnimState, playerData);

        nowState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::idle;
    }

    //走る
    if (((playerData.isMove && !playerData.isRoll && playerData.isSprint &&
        !playerData.isSlip && playerData.isGround) ||
        playerData.isRun_wall) && animNumber_Now != animNum::run)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //nowStateの中身を削除
        nowState = nullptr;

        playerData.isRun = true;
        playerData.isStopRun = false;
        playerData.isIdle = false;

        //nowStateを更新
        nowState = std::make_shared<Run>(modelHandle, oldAnimState, nowAnimState);
        
        nowState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::run;
    }

    //ジャンプ
    if (playerData.isJump_PlayAnim && (playerData.isJump || playerData.isJump_second) &&
        animNumber_Now != animNum::jump)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;

        //nowStateを更新
        nowState = std::make_shared<Jump>(modelHandle, oldAnimState, nowAnimState);
       
        nowState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::jump;

        //接地中のロールアクションの場合はもう一度使えるように
        if (playerData.isGround)
        {
            playerData.isRollFinished = false;
            playerData.isRoll_PlayAnim = false;
        }
        playerData.isIdle = false;
        playerData.isRoll = false;
        playerData.isJump_run_playAnim = false;
        playerData.isSlip = false;
    }

    //ランジャンプ
    if (playerData.isJump_run_playAnim && (playerData.isJump || playerData.isJump_second) &&
        !playerData.isSlip && animNumber_Now != animNum::run_Jump)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        // prevAttachIndex = nowState->GetPrevAttachIndex();
        nowState = nullptr;
        nowState = std::make_shared<Run_Jump>(modelHandle, oldAnimState, nowAnimState);

        nowState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::run_Jump;

        //接地中のロールアクションの場合はもう一度使えるように
        if (playerData.isGround)
        {
            playerData.isRollFinished = false;
            playerData.isRoll_PlayAnim = false;
        }
        playerData.isIdle = false;
        playerData.isRoll = false;
        playerData.isJump_PlayAnim = false;
        playerData.isSlip = false;
    }

    //落下中
    if (isChangeState && !playerData.isGround && !playerData.isRun_wall &&
        animNumber_Now != animNum::falling_Idle)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;
        animNumber_Now = animNum::falling_Idle;
        nowState = std::make_shared<Falling_Idle>(modelHandle, oldAnimState, nowAnimState);
        playerData.isFalling = true;
        playerData.isRoll_PlayAnim = false;
        playerData.isRollFinished = true;
    }

    //転がる
    if (playerData.isRoll && !playerData.isRoll_PlayAnim &&
        !playerData.isRollFinished && animNumber_Now != animNum::quick_Roll)
    {

        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;
        nowState = std::make_shared<Quick_Roll>(modelHandle, oldAnimState, nowAnimState);
        
        nowState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::quick_Roll;

        playerData.isJump_second = false;
        playerData.isJumpAll = false;
        playerData.isJump_PlayAnim = false;
        playerData.isJump_run_playAnim = false;
        playerData.isRoll_PlayAnim = true;
        playerData.isSprint = true;
        playerData.isRun = true;
        playerData.isIdle = false;
        playerData.isStopRun = false;
        playerData.isSlip = false;
    }

    //走り出し
    if (!playerData.isSprint && playerData.isMove &&
        !playerData.isRoll && !playerData.isJump &&
        animNumber_Now != animNum::idle_To_Sprint)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;
        nowState = std::make_shared<Idle_To_Sprint>(modelHandle, oldAnimState, nowAnimState);

        nowState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::idle_To_Sprint;

        playerData.isIdle = false;
    }

    //走り終わり
    if (!playerData.isStopRun && !playerData.isIdle && !playerData.isMove &&
        playerData.isGround && !playerData.isRoll && !playerData.isJump &&
        playerData.isRun &&
        animNumber_Now != animNum::run_To_Stop)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;
        playerData.isStopRun = true;
        playerData.isRun = false;
        nowState = std::make_shared<Run_To_Stop>(modelHandle, oldAnimState, nowAnimState);
      
        nowState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::run_To_Stop;
    }

    //崖つかみ
    if (playerData.isHanging && playerData.isFalling &&
        animNumber_Now != animNum::hanging_Idle)
    {
        SetOldAnimState(nowState->GetOldAnimState());
        SetNowAnimState(nowState->GetNowAnimState());

        nowState = nullptr;
        nowState = std::make_shared<Hanging_Idle>(modelHandle, oldAnimState, nowAnimState, playerData);
        
        nowState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::hanging_Idle;

        playerData.isFalling = false;
        playerData.isIdle = false;
    }

    //崖のぼり
    if (playerData.isHanging && playerData.isHang_to_Crouch &&
        animNumber_Now != animNum::braced_Hang_To_Crouch)
    {
        SetOldAnimState(nowState->GetOldAnimState());
        SetNowAnimState(nowState->GetNowAnimState());

        nowState = nullptr;
        nowState = std::make_shared<Braced_Hang_To_Crouch>(modelHandle, oldAnimState, nowAnimState, playerData);

        nowState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::braced_Hang_To_Crouch;

        playerData.isHanging = false;
        playerData.isIdle = false;
    }

    //急回転
    if (playerData.isSlip && animNumber_Now != animNum::running_turn_left)
    {
        SetOldAnimState(nowState->GetOldAnimState());
        SetNowAnimState(nowState->GetNowAnimState());

        nowState = nullptr;
        nowState = std::make_shared<Running_turn>(modelHandle, oldAnimState, nowAnimState, playerData);
        
        nowState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::running_turn_left;

        playerData.isRun = false;
    }

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