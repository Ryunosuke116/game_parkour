#include "Common.h"
#include <memory>
#include "AnimNum.h"
#include "PlayerState.h"
#include "PadInput.h"
#include "AnimationChanger.h"
#include "Player.h"
#include "GameInstanceSubSystem.h"
#include "SoundPlayer.h"

AnimationChanger::AnimationChanger():
    nowAnimNumber(-1)
{

}

AnimationChanger::~AnimationChanger()
{
    stateList.clear();
}

void AnimationChanger::Create(const int modelHandle)
{
    stateList[animNum::idle]                = std::make_shared<Idle>                (modelHandle);
    stateList[animNum::bracedHangToCrouch]  = std::make_shared<BracedHangToCrouch>  (modelHandle);
    stateList[animNum::fallingIdle]         = std::make_shared<FallingIdle>         (modelHandle);
    stateList[animNum::hangingIdle]         = std::make_shared<HangingIdle>         (modelHandle);
    stateList[animNum::jump]                = std::make_shared<Jump>                (modelHandle);
    stateList[animNum::quickRoll]           = std::make_shared<QuickRoll>           (modelHandle);
    stateList[animNum::run]                 = std::make_shared<Run>                 (modelHandle);
    stateList[animNum::runToStop]           = std::make_shared<RunToStop>           (modelHandle);
    stateList[animNum::walk]                = std::make_shared<Walk>                (modelHandle);
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

    nowState->SetOldAnimNumber(nowAnimNumber);
    nowAnimNumber = animNum::walk;

    nowState->Initialize(modelHandle, nowAnimNumber, player);
}

void AnimationChanger::ResultInitialize(
    const int num, 
    const int modelHandle,
    std::shared_ptr<PlayerStateBase>& nowState,
    PlayerData& playerData,
    Player& player)
{
    nowAnimNumber = num;

    //newStateを生成
    nowState = std::make_shared<Victory>(modelHandle);

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
    auto Change = [&](
        const int animNumber,
        PlayerData & playerData)
    {
        //newStateを生成
        newState = stateList.at(animNumber);

        newState->SetOldAnimNumber(nowAnimNumber);
        nowAnimNumber = animNumber;
    };

    //立ち
    if (playerData.isIdle && 
        nowAnimNumber != animNum::idle)
    {
         Change(animNum::idle, playerData);
    }

    //歩く
    if (playerData.isWalk &&
        nowAnimNumber != animNum::walk)
    {
        Change(animNum::walk, playerData);
    }

    //走る
    if (playerData.isRun &&
        nowAnimNumber != animNum::run)
    {
        Change(animNum::run, playerData);
    }

    //ジャンプ
    if (playerData.isJump &&
        nowAnimNumber != animNum::jump &&
        nowAnimNumber != animNum::secondJump)
    {
        Change(animNum::jump, playerData);
    }

    //落下中
    if (playerData.isFalling &&
        nowAnimNumber != animNum::fallingIdle)
    {
        Change(animNum::fallingIdle, playerData);
    }

    //転がる
    if (playerData.isRoll &&
        nowAnimNumber != animNum::quickRoll)
    {
        Change(animNum::quickRoll, playerData);
    }

    //走り終わり
    if (playerData.isStopRun && 
        nowAnimNumber != animNum::runToStop)
    {
        Change(animNum::runToStop, playerData);
    }

    //崖つかみ
    if (playerData.isHanging &&
        nowAnimNumber != animNum::hangingIdle)
    {
        Change(animNum::hangingIdle, playerData);
    }

    //崖のぼり
    if (playerData.isHangToCrouch &&
        nowAnimNumber != animNum::bracedHangToCrouch)
    {
        Change(animNum::bracedHangToCrouch, playerData);
    }

    if (newState)
    {
        newState->Enter(nowState->GetOldAnimState(), nowState->GetNowAnimState());
        newState->Initialize(modelHandle, nowAnimNumber, player);
        return newState;
    }

    return nowState;
}
