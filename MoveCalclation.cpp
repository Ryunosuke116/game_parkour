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

/// @brief 
/// @param moveVec 
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

void MoveCalclation::Gravity(const VECTOR& moveVec, const PlayerStateActionBase::PlayerData& playerData)
{
    if (!playerData.isGround)
    {
        currentJumpSpeed += gravity;
    }
}

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

VECTOR MoveCalclation::MoveVec()
{
    bool isJump, isRoll;
    VECTOR moveVec, newMoveVec;

    if (isJump || isRoll)
    {
        moveVec = Calclation::Leap(moveVec, newMoveVec, 0.2f);
    }
}