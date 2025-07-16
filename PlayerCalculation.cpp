#include "Dxlib.h"
#include "Calculation.h"
#include "PlayerCalculation.h"
#include"playerState.h"
#include "AnimTime.h"
#include "HitCheck.h"


PlayerCalculation::PlayerCalculation() :
    jumpSpeed_now(0.0f),
    moveSpeed_now(0.0f),
    rollMoveSpeed_now(0.0f),
    isCalc_deceleration(false)
{

}

VECTOR PlayerCalculation::Update(const VECTOR& moveVec, const VECTOR& moveDirection, const float playTime_anim,
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
VECTOR PlayerCalculation::Move(const VECTOR& moveVec, const VECTOR& moveDirection, const PlayerStateActionBase::PlayerData& playerData)
{
    if (playerData.isMove)
    {
        if (isCalc_deceleration)
        {
            isCalc_deceleration = false;
        }

        //徐々にスピードを上げる
        moveSpeed_now += 0.05f;

        //maxに達したらそこで止める
        if (moveSpeed_now >= MaxMoveSpeed)
        {
            moveSpeed_now = MaxMoveSpeed;
        }
    }
    else
    {
        //接地しているときに止まったらすぐ止まる
        if (playerData.isGround)
        {
            //アニメーションフレームに合わせて減速
            if (!isCalc_deceleration)
            {
                const float stopFrame = 10.0f;
                decelerationSpeed = moveSpeed_now / stopFrame;
                isCalc_deceleration = true;
            }

            moveSpeed_now -= decelerationSpeed;

            if (moveSpeed_now < 0.0f)
            {
                moveSpeed_now = 0.0f;
            }
        }
        //空中にいるとき
        else
        {
            //徐々に下げる
            moveSpeed_now -= 0.02f;

            //止める
            if (moveSpeed_now <= 0.0f)
            {
                moveSpeed_now = 0.0f;
            }
        }
    }

    return VScale(moveDirection, moveSpeed_now);
}

/// <summary>
/// ジャンプ力計算
/// </summary>
/// <param name="moveVec"></param>
/// <param name="animNumber_Now"></param>
/// <param name="playerData"></param>
/// <returns></returns>
VECTOR PlayerCalculation::Jump(const VECTOR& moveVec,const int& animNumber_Now,
    const PlayerStateActionBase::PlayerData& playerData)
{
    VECTOR move = moveVec;

    if (animNumber_Now == animNum::falling_Idle ||
        (playerData.isJump ||
        animNumber_Now == animNum::run_Jump))
    {
        move.y += jumpSpeed_now;
    }
    else if (!playerData.isGround)
    {
        move.y += jumpSpeed_now;
    }

    return move;
}

/// <summary>
/// 重力計算
/// </summary>
/// <param name="moveVec"></param>
/// <param name="playerData"></param>
void PlayerCalculation::Gravity(const VECTOR& moveVec, const PlayerStateActionBase::PlayerData& playerData)
{
    if (!playerData.isGround)
    {
        jumpSpeed_now += gravity;
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
VECTOR PlayerCalculation::Roll(const VECTOR& moveVec, const VECTOR& moveDirection,
    float playTime_anim, const PlayerStateActionBase::PlayerData& playerData)
{
    VECTOR move = moveVec;

    if (playerData.isRoll)
    {
        if (playTime_anim >= 10.0f)
        {
            //※徐々にスピードを下げていく
            //途中で向きがあまり変更できないようにする
            move = moveDirection;
        }
    }

    return move;
}

/// <summary>
/// 崖に掴まっているときの方向計算
/// </summary>
/// <param name="hangingPoly"></param>
/// <returns></returns>
VECTOR PlayerCalculation::HangingAngle(const MV1_COLL_RESULT_POLY& hangingPoly)
{
    //張り付く壁の法線ベクトルを利用してplayerの向きを調整
    VECTOR direction = hangingPoly.Normal;
    direction.y = 0.0f;

    direction = VScale(direction, -1.0f);


    return VNorm(direction);
}

/// <summary>
/// 掴まる場所計算
/// </summary>
/// <param name="handPos_left"></param>
/// <param name="handPos_right"></param>
/// <param name="nearestPoint"></param>
/// <returns></returns>
VECTOR PlayerCalculation::HangingPosition(const VECTOR& handPos_left,const VECTOR& handPos_right,const VECTOR& nearestPoint)
{
    VECTOR centerPos = VAdd(handPos_left, handPos_right);
    centerPos = VScale(centerPos, 0.5f);
    
    VECTOR newPos = VSub(nearestPoint, centerPos);
    return newPos;
}

VECTOR PlayerCalculation::HangingDirection(const MV1_COLL_RESULT_POLY& hangingPoly, const VECTOR& centerPos)
{
    //点に最も近い線分との最近接点
    nearestResult = Calculation::SphereMeshOutsideTriangle_line(hangingPoly, centerPos);

    //射影ベクトル
    VECTOR a = Calculation::ProjectionDirection(centerPos, nearestResult.linePos_start, nearestResult.linePos_end);

    VECTOR direction = VSub(a, centerPos);
    direction.y = 0.0f;

    direction = VNorm(direction);

    return direction;
}

void PlayerCalculation::Reset_move()
{
    moveSpeed_now = 0.0f;
    jumpSpeed_now = 0.0f;
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
VECTOR PlayerCalculation::MoveVec(const VECTOR& moveVec,const VECTOR& moveVec_memory,const bool isGround,const bool isRoll)
{
    VECTOR returnPos = moveVec;

    if (isGround || !isRoll)
    {
        returnPos = moveVec_memory;
    }

    if (!isGround || isRoll)
    {
        returnPos = Calculation::Leap(moveVec, moveVec_memory, 0.2f);
    }

    return returnPos;
}

