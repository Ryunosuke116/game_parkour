#include "Dxlib.h"
#include "Calculation.h"
#include "PlayerData.h"
#include "PlayerCalculation.h"
#include"playerState.h"
#include "AnimTime.h"
#include "HitCheck.h"
#include "DebugDrawer.h"



PlayerCalculation::PlayerCalculation() :
    moveVec_old(VGet(0.0f, 0.0f, 0.0f)),
    jumpPower_now(0.0f),
    moveSpeed_now(0.0f),
    rollMoveSpeed_now(0.0f),
    wallRun_stopTime(0.0f),
    isCalc_deceleration(false),
    isSlip_after(false),
    isRun_Wall_Stop(false),
    isJumpPower_add(false)
{

}

VECTOR PlayerCalculation::Update(const VECTOR& moveDirection, 
    const float playTime_anim, const int& animNumber_Now, 
    const PlayerData& playerData)
{
    VECTOR moveVec = moveDirection;

    //進むスピードを乗算
      //ロールアクション中はそれに応じた速度
    moveVec = Roll(animNumber_Now, moveVec, moveDirection,
        playTime_anim, playerData);
   
    //通常時
    moveVec = Move(animNumber_Now, moveDirection, moveVec, playerData);

    //崖掴み中
    if (playerData.isHanging)
    {
        moveVec = HangingPosition();
    }


    return moveVec;
}

/// <summary>
/// 移動距離計算
/// </summary>
/// <param name="moveVec"></param>
/// <param name="moveDirection"></param>
/// <param name="playerData"></param>
/// <returns></returns>
VECTOR PlayerCalculation::Move(const int& animNumber_Now,
    const VECTOR& moveDirection, const VECTOR& moveVec, 
    const PlayerData& playerData)
{

    if (playerData.isHanging) return moveVec;

    VECTOR returnVec = moveVec;

    if (!playerData.isRoll)
    {
        //動いている場合移動スピードを徐々に上げる
        if (playerData.isMove && !playerData.isSlip)
        {
            //減速スピード計算していたらfalseに
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
                    //stopアニメーションに応じて止まるタイミングを調整
                    if (!playerData.isSlip)
                    {
                        const float stopFrame = 10.0f;
                        decelerationSpeed = moveSpeed_now / stopFrame;
                        isCalc_deceleration = true;
                    }
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

        returnVec = VScale(moveDirection, moveSpeed_now);

        //急転回している場合
        if (playerData.isSlip)
        {
            return VScale(moveDirection, -moveSpeed_now);
        }
    }


    //重力だけ前フレームのモノを使用
    returnVec.y = moveVec_old.y;

    //重力計算
    returnVec = Gravity(returnVec, playerData);

    //ジャンプ計算
    if (playerData.isRun_wall)
    {
        returnVec = Run_Wall(returnVec, playerData);
    }
    else
    {
        returnVec = Jump(returnVec, animNumber_Now, playerData);
    }

    //moveVecを保存
    moveVec_old = returnVec;

    return returnVec;
}

/// <summary>
/// ジャンプ力計算
/// </summary>
/// <param name="moveVec"></param>
/// <param name="animNumber_Now"></param>
/// <param name="playerData"></param>
/// <returns></returns>
VECTOR PlayerCalculation::Jump(const VECTOR& moveVec,const int& animNumber_Now,
    const PlayerData& playerData)
{
    VECTOR move = moveVec;

    if (isJumpPower_add)
    {
        if (playerData.isJump_second)
        {
            move.y = 0.0f;
        }
        move.y += jumpPower_now;
        isJumpPower_add = false;
    }

    return move;
}

/// <summary>
/// 重力計算
/// </summary>
/// <param name="moveVec"></param>
/// <param name="playerData"></param>
VECTOR PlayerCalculation::Gravity(const VECTOR& moveVec, const PlayerData& playerData)
{
    VECTOR returnVec = moveVec;

    if (!playerData.isGround)
    {
        //velocity_gravity += gravity;

        returnVec.y += gravity;
    }
    else
    {
        velocity_gravity = 0.0f;
        returnVec.y = 0.0f;
    }

    //接地中は重力を付与しない
    return returnVec;
}

/// <summary>
/// ロール距離計算
/// </summary>
/// <param name="moveVec"></param>
/// <param name="moveDirection"></param>
/// <param name="playTime_anim"></param>
/// <param name="playerData"></param>
/// <returns></returns>
VECTOR PlayerCalculation::Roll(const int& animNumber_Now, 
    const VECTOR& moveVec, const VECTOR& moveDirection,
    float playTime_anim, const PlayerData& playerData)
{
    VECTOR move = moveVec;

    if (playerData.isRoll)
    {
        //特定のフレームまで移動しない
        if (playTime_anim >= 10.0f)
        {
            //※徐々にスピードを下げていく
            //途中で向きがあまり変更できないようにする
            move = moveDirection;
        }

        if (playTime_anim >= 10.0f ||
            animNumber_Now != animNum::quick_Roll)
        {
            move = VScale(moveVec, rollMoveSpeed_max);
            moveSpeed_now = MaxMoveSpeed;
        }
    }

    return move;
}

VECTOR PlayerCalculation::Run_Wall(const VECTOR& moveVec,
    const PlayerData& playerData)
{
    VECTOR move = moveVec;

    //一度だけジャンプ力を付与
    if (isJumpPower_add)
    {
        isRun_Wall_Stop = false;
        wallRun_stopTime = 0.0f;
        move.y = 0.0f;
        move.y += jumpPower_now;
        isJumpPower_add = false;
    }
    
    //ジャンプ力が0になったら少し留まる
    if (move.y <= 0.0f && !isRun_Wall_Stop)
    {
        isRun_Wall_Stop = true;
        move.y = 0.0f;
    }

    //少しの間留まる
    if (isRun_Wall_Stop)
    {
        if (wallRun_stopTime <= wallRun_stopTime_max)
        {
            wallRun_stopTime++;
            move.y = 0.0f;
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
VECTOR PlayerCalculation::HangingPosition()
{
    VECTOR centerPos = VAdd(handPos_left, handPos_right);
    centerPos = VScale(centerPos, 0.5f);
    
    VECTOR newPos = VSub(nearestResult.nearestPoint, centerPos);
    DebugDrawer::Instance().InformationInput_sphere(nearestResult.nearestPoint, 2.0f, GetColor(0, 255, 255));
    return newPos;
}

/// <summary>
/// 掴んでいる場所の方向を計算
/// </summary>
/// <param name="hangingPoly"></param>
/// <param name="centerPos"></param>
/// <returns></returns>
VECTOR PlayerCalculation::HangingDirection(const MV1_COLL_RESULT_POLY& hangingPoly, const VECTOR& centerPos)
{
    //点に最も近い線分との最近接点
    nearestResult = Calculation::SphereMeshOutsideTriangle_line(hangingPoly, centerPos);

    //射影ベクトル
    VECTOR a = Calculation::ProjectionDirection(centerPos, nearestResult.linePos_start, nearestResult.linePos_end);

    DebugDrawer::Instance().InformationInput_line(nearestResult.linePos_start, nearestResult.linePos_end, GetColor(255, 0, 0));
    DebugDrawer::Instance().InformationInput_sphere(a, 2.0f, GetColor(0, 255, 0));

    VECTOR direction = VSub(a, centerPos);
    direction.y = 0.0f;

    direction = VNorm(direction);

    return direction;
}


void PlayerCalculation::Reset_move()
{
    moveSpeed_now = 0.0f;
    jumpPower_now = 0.0f;
}

void PlayerCalculation::Reset_run_wall()
{
    wallRun_stopTime = 0.0f;
    isRun_Wall_Stop = false;
}


