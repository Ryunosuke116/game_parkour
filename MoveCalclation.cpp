#include "Dxlib.h"
#include "Calclation.h"
#include "MoveCalclation.h"
#include"playerState.h"
#include "AnimTime.h"


MoveCalclation::MoveCalclation() :
    currentJumpSpeed(0.0f),
    nowMoveSpeed(0.0f),
    rollMoveSpeed_now(0.0f)
{

}

VECTOR MoveCalclation::Update(const VECTOR& moveVec, const VECTOR& moveDirection, const float playTime_anim,
    const int& animNumber_Now, const PlayerStateActionBase::PlayerData& playerData)
{
    VECTOR returnVec = moveVec;

    returnVec = Roll(returnVec, moveDirection, playTime_anim, playerData);
    //進むスピードを乗算
      //ロールアクション中はそれに応じた速度
    if (playerData.isRoll)
    {
        returnVec = VScale(returnVec, rollMoveSpeed_max);
    }
    else
    {
        returnVec = Move(returnVec, moveDirection, playerData);
    }

    //ジャンプ計算
    returnVec = Jump(returnVec, animNumber_Now, playerData);

    //重力計算
    Gravity(returnVec, playerData);

    return returnVec;
}

/// <summary>
/// 移動距離計算
/// </summary>
/// <param name="moveVec"></param>
/// <param name="moveDirection"></param>
/// <param name="playerData"></param>
/// <returns></returns>
VECTOR MoveCalclation::Move(const VECTOR& moveVec, const VECTOR& moveDirection, const PlayerStateActionBase::PlayerData& playerData)
{
    if (playerData.isMove)
    {
        //徐々にスピードを上げる
        nowMoveSpeed += 0.05f;

        //maxに達したらそこで止める
        if (nowMoveSpeed >= MaxMoveSpeed)
        {
            nowMoveSpeed = MaxMoveSpeed;
        }
    }
    else
    {
        //接地しているときに止まったらすぐ止まる
        if (playerData.isGround)
        {
            nowMoveSpeed = 0.0f;
        }
        //空中にいるとき
        else
        {
            //徐々に下げる
            nowMoveSpeed -= 0.02f;

            //止める
            if (nowMoveSpeed <= 0.0f)
            {
                nowMoveSpeed = 0.0f;
            }
        }
    }

    return VScale(moveDirection, nowMoveSpeed);
}

/// <summary>
/// ジャンプ力計算
/// </summary>
/// <param name="moveVec"></param>
/// <param name="animNumber_Now"></param>
/// <param name="playerData"></param>
/// <returns></returns>
VECTOR MoveCalclation::Jump(const VECTOR& moveVec,const int& animNumber_Now,
    const PlayerStateActionBase::PlayerData& playerData)
{
    VECTOR move = moveVec;

    if (animNumber_Now == animNum::falling_Idle || (playerData.isJump && animNumber_Now == animNum::jump ||
        animNumber_Now == animNum::run_Jump))
    {
        move.y += currentJumpSpeed;
    }
    else if (!playerData.isGround)
    {
        move.y += currentJumpSpeed;
    }

    return move;
}

/// <summary>
/// 重力計算
/// </summary>
/// <param name="moveVec"></param>
/// <param name="playerData"></param>
void MoveCalclation::Gravity(const VECTOR& moveVec, const PlayerStateActionBase::PlayerData& playerData)
{
    if (!playerData.isGround)
    {
        currentJumpSpeed += gravity;
    }
}

/// <summary>
/// ロール距離計算
/// </summary>
/// <param name="moveVec"></param>
/// <param name="moveDirection"></param>
/// <param name="playTime_anim"></param>
/// <param name="playerData"></param>
/// <returns></returns>
VECTOR MoveCalclation::Roll(const VECTOR& moveVec, const VECTOR& moveDirection,
    float playTime_anim, const PlayerStateActionBase::PlayerData& playerData)
{
    VECTOR move = moveVec;

    if (playerData.isRoll)
    {
        if (playTime_anim >= 10.0f)
        {
            //※徐々にスピードを下げていく
            //途中で向きがあまり帰れないようにする
            move = moveDirection;
        }
    }

    return move;
}


///////////////////////////////
// まだ使うかわからない
//////////////////////////////

/// <summary>
/// 
/// </summary>
/// <param name="moveVec"></param>
/// <param name="moveVec_memory"></param>
/// <param name="isGround"></param>
/// <param name="isRoll"></param>
/// <returns></returns>
VECTOR MoveCalclation::MoveVec(const VECTOR& moveVec,const VECTOR& moveVec_memory,const bool isGround,const bool isRoll)
{
    VECTOR returnPos = moveVec;

    if (isGround || !isRoll)
    {
        returnPos = moveVec_memory;
    }

    if (!isGround || isRoll)
    {
        returnPos = Calclation::Leap(moveVec, moveVec_memory, 0.2f);
    }

    return returnPos;
}