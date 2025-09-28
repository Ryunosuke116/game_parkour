#include "common.h"
#include <memory>
#include "AnimTime.h"
#include "playerState.h"
#include "PadInput.h"
#include "AnimationChanger.h"
#include "Player.h"
#include "GameInstanceSubSystem.h"
#include "SoundPlayer.h"

AnimationChanger::AnimationChanger()
{
}

AnimationChanger::~AnimationChanger()
{

}

void AnimationChanger::Initialize(
    const int& num, 
    int& modelHandle,
    std::shared_ptr<PlayerStateBase>& nowState,
    PlayerData& playerData, 
    Player& player)
{
    animNumber_Now = num;
    //newStateを生成
    nowState = std::make_shared<Walk>(modelHandle,
        oldAnimState,
        nowAnimState);

    nowState->SetAnimNumber_old(animNumber_Now);
    animNumber_Now = animNum::walk;

    nowState->Initialize(modelHandle, animNumber_Now,player);
    nowState->Enter(playerData);
}

void AnimationChanger::ResultInitialize(
    const int& num, 
    int& modelHandle,
    std::shared_ptr<PlayerStateBase>& nowState,
    PlayerData& playerData,
    Player& player)
{
    animNumber_Now = num;
    //newStateを生成
    nowState = std::make_shared<Victory>(modelHandle,
        oldAnimState,
        nowAnimState,
        playerData);

    nowState->SetAnimNumber_old(animNumber_Now);

      nowState->Initialize(modelHandle, animNumber_Now, player);
    nowState->Enter(playerData);
}

/// <summary>
/// アニメーション変更
/// </summary>
std::shared_ptr<PlayerStateBase> AnimationChanger::ChangeState(
    int& modelHandle,
    Player& player, 
    PlayerData& playerData,
    std::shared_ptr<PlayerStateBase>& nowState)
{
    std::shared_ptr<PlayerStateBase> newState = nullptr;
    const auto soundPlayer = 
        GameInstanceSubSystem::GetInstance().GetSubSystem<SoundPlayer>().lock();

    //立ち
    if (playerData.isIdle && 
        animNumber_Now != animNum::idle)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Idle>(modelHandle,
            oldAnimState,
            nowAnimState,
           
            playerData);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::idle;
    }

    //歩く
    if (playerData.isWalk &&
        animNumber_Now != animNum::walk)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Walk>(modelHandle,
            oldAnimState,
            nowAnimState);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::walk;
    }

    //走る
    if (playerData.isRun && animNumber_Now != animNum::run)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Run>(modelHandle,
            oldAnimState,
            nowAnimState);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::run;
        soundPlayer->Play("dash");
    }

    //ジャンプ
    if (playerData.isJump &&
        animNumber_Now != animNum::jump &&
        animNumber_Now != animNum::runJump)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Jump>(modelHandle,
            oldAnimState,
            nowAnimState,
            playerData);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::jump;
    }

    //落下中
    if (playerData.isFalling &&
        animNumber_Now != animNum::fallingIdle)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<FallingIdle>(modelHandle,
            oldAnimState,
            nowAnimState);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::fallingIdle;
    }

    //転がる
    if (playerData.IsPushRT && animNumber_Now != animNum::quickRoll)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<QuickRoll>(modelHandle,
            oldAnimState,
            nowAnimState);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::roll;
    }

    //走り出し
    if (playerData.isSprint && animNumber_Now != animNum::idleToSprint)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<IdleToSprint>(modelHandle,
            oldAnimState,
            nowAnimState);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::idleToSprint;
    }

    //走り終わり
    if (playerData.isStopRun && animNumber_Now != animNum::runToStop)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<RunToStop>(modelHandle,
            oldAnimState,
            nowAnimState);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::runToStop;
    }

    //崖つかみ
    if (playerData.isHanging && animNumber_Now != animNum::hangingIdle)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<HangingIdle>(modelHandle,
            oldAnimState,
            nowAnimState);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::hangingIdle;
    }

    //崖のぼり
    if (playerData.isHangToCrouch &&
        animNumber_Now != animNum::bracedHangToCrouch)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<BracedHangToCrouch>(modelHandle,
            oldAnimState,
            nowAnimState);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::bracedHangToCrouch;
    }

    if (newState)
    {
        newState->Initialize(modelHandle, animNumber_Now, player);
        newState->Enter(playerData);
        if (!playerData.isRun)
        {
            soundPlayer->Stop("dash");
        }
        return newState;
    }

    return nowState;
}

/// <summary>
/// アニメーション情報をセット
/// </summary>
/// <param name="AnimState"></param>
void AnimationChanger::SetOldAnimState(PlayerStateBase::AnimState animState)
{
    oldAnimState.AttachIndex = animState.AttachIndex;
    oldAnimState.PlayAnimSpeed = animState.PlayAnimSpeed;
    oldAnimState.PlayTime_anim = animState.PlayTime_anim;
    oldAnimState.TotalPlayTime_anim = animState.TotalPlayTime_anim;
}

void AnimationChanger::SetNowAnimState(PlayerStateBase::AnimState animState)
{
    nowAnimState.AttachIndex = animState.AttachIndex;
    nowAnimState.PlayAnimSpeed = animState.PlayAnimSpeed;
    nowAnimState.PlayTime_anim = animState.PlayTime_anim;
    nowAnimState.TotalPlayTime_anim = animState.TotalPlayTime_anim;
}