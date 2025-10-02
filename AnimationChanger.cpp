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
    stateList.clear();
}

void AnimationChanger::Create(const int modelHandle)
{
    stateList[animNum::idle] = std::make_shared<Idle>(modelHandle);
    stateList[animNum::bracedHangToCrouch] = std::make_shared<BracedHangToCrouch>(modelHandle);
    stateList[animNum::fallingIdle] = std::make_shared<FallingIdle>(modelHandle);
    stateList[animNum::hangingIdle] = std::make_shared<HangingIdle>(modelHandle);
    stateList[animNum::jump] = std::make_shared<Jump>(modelHandle);
    stateList[animNum::quickRoll] = std::make_shared<QuickRoll>(modelHandle);
    stateList[animNum::run] = std::make_shared<Run>(modelHandle);
    stateList[animNum::runToStop] = std::make_shared<RunToStop>(modelHandle);
    stateList[animNum::walk] = std::make_shared<Walk>(modelHandle);
}

void AnimationChanger::Initialize(
    const int& num, 
    const int modelHandle,
    std::shared_ptr<PlayerStateBase>& nowState,
    PlayerData& playerData, 
    Player& player)
{
    nowAnimNumber = num;

    //nowStateにwalkStateをセット
	nowState = stateList[nowAnimNumber];
    nowState->Enter(oldAnimState,
        nowAnimState);

    nowState->SetOldAnimNumber(nowAnimNumber);
    nowAnimNumber = animNum::walk;

    nowState->Initialize(modelHandle, nowAnimNumber, player);
}

void AnimationChanger::ResultInitialize(const int num, 
    const int modelHandle,
    std::shared_ptr<PlayerStateBase>& nowState,
    PlayerData& playerData,
    Player& player)
{
    nowAnimNumber = num;

    //newStateを生成
    nowState = std::make_shared<Victory>(modelHandle);
    nowState->Enter(oldAnimState,
        nowAnimState);

    nowState->SetOldAnimNumber(nowAnimNumber);

    nowState->Initialize(modelHandle, nowAnimNumber, player);
}

/// <summary>
/// アニメーション変更
/// </summary>
std::shared_ptr<PlayerStateBase> AnimationChanger::ChangeState(
    const int modelHandle,
    Player& player, 
    PlayerData& playerData,
    std::shared_ptr<PlayerStateBase>& nowState)
{
    std::shared_ptr<PlayerStateBase> newState = nullptr;

    // 新しいstateを生成するラムダ関数を用意
    auto Change = [&](const int animNumber,
        Player & player,
        PlayerData & playerData)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = stateList.at(animNumber);

        newState->SetOldAnimNumber(nowAnimNumber);
        nowAnimNumber = animNumber;
    };

    //立ち
    if (playerData.isIdle && 
        nowAnimNumber != animNum::idle)
    {
         Change(animNum::idle, player, playerData);
    }

    //歩く
    if (playerData.isWalk &&
        nowAnimNumber != animNum::walk)
    {
        Change(animNum::walk, player, playerData);
    }

    //走る
    if (playerData.isRun && nowAnimNumber != animNum::run)
    {
        Change(animNum::run, player, playerData);
    }

    //ジャンプ
    if (playerData.isJump &&
        nowAnimNumber != animNum::jump &&
        nowAnimNumber != animNum::runJump)
    {
        Change(animNum::jump, player, playerData);
    }

    //落下中
    if (playerData.isFalling &&
        nowAnimNumber != animNum::fallingIdle)
    {
        Change(animNum::fallingIdle, player, playerData);
    }

    //転がる
    if (playerData.isRoll &&
        nowAnimNumber != animNum::quickRoll)
    {
        Change(animNum::quickRoll, player, playerData);
    }

    //走り終わり
    if (playerData.isStopRun && 
        nowAnimNumber != animNum::runToStop)
    {
        Change(animNum::runToStop, player, playerData);
    }

    //崖つかみ
    if (playerData.isHanging &&
        nowAnimNumber != animNum::hangingIdle)
    {
        Change(animNum::hangingIdle, player, playerData);
    }

    //崖のぼり
    if (playerData.isHangToCrouch &&
        nowAnimNumber != animNum::bracedHangToCrouch)
    {
        Change(animNum::bracedHangToCrouch, player, playerData);
    }

    if (newState)
    {
        newState->Enter(oldAnimState, nowAnimState);
        newState->Initialize(modelHandle, nowAnimNumber, player);
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
    oldAnimState.attachIndex = animState.attachIndex;
    oldAnimState.PlayAnimSpeed = animState.PlayAnimSpeed;
    oldAnimState.playAnimTime = animState.playAnimTime;
    oldAnimState.TotalPlayTime_anim = animState.TotalPlayTime_anim;
}

void AnimationChanger::SetNowAnimState(PlayerStateBase::AnimState animState)
{
    nowAnimState.attachIndex = animState.attachIndex;
    nowAnimState.PlayAnimSpeed = animState.PlayAnimSpeed;
    nowAnimState.playAnimTime = animState.playAnimTime;
    nowAnimState.TotalPlayTime_anim = animState.TotalPlayTime_anim;
}