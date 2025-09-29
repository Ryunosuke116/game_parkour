#include "Dxlib.h"
#include "Calculation.h"
#include "PlayerData.h"
#include "PlayerCalculation.h"
#include"playerState.h"
#include "AnimTime.h"
#include "HitCheck.h"
#include "DebugDrawer.h"
#include "WorldSubSystem.h"
#include "CollisionObjectManager.h"
#include "Player.h"
#include "PlayerManager.h"




PlayerCalculation::PlayerCalculation() :
    moveVec_old(VGet(0.0f, 0.0f, 0.0f)),
    hitWallNormal(VGet(0.0f, 0.0f, 0.0f)),
    jumpPower_now(0.0f),
    moveSpeed_now(0.0f),
    rollMoveSpeed_now(0.0f),
    wallRun_stopTime(0.0f),
    isCalc_deceleration(false),
    isSlip_after(false),
    isRunWall_Stop(false),
    isJumpPower_add(false)
{

}

VECTOR PlayerCalculation::Update(
    const VECTOR& moveDirection, 
    const float playTime_anim,
    const int animNumber_Now, 
    const PlayerData& playerData)
{
    VECTOR moveVec = moveDirection;

    //進むスピードを乗算
      //ロールアクション中はそれに応じた速度
    moveVec = Roll(animNumber_Now,
        moveVec,
        moveDirection,
        playTime_anim,
        playerData);
   
    //通常時
    moveVec = Move(animNumber_Now, moveDirection, moveVec, playerData);

    //崖掴み中
    if (playerData.isHanging_now)
    {
        moveVec = HangingPosition();
    }

    if (playerData.isHangToCrouch)
    {
        moveVec = HangToCrouchMove(
            WorldSubSystem::GetInstance().GetSubSystem<CollisionObjectManager>()->GetCollisionObjects(),
            WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>()->GetPlayer());
    }
    else
    {
        isWhenClimbingHitGround = false;
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

    VECTOR velocity = moveVec;

    if (!playerData.IsPushRT)
    {
        //動いている場合移動スピードを徐々に上げる
        if (playerData.isMove)
        {
            //減速スピード計算していたらfalseに
            if (isCalc_deceleration)
            {
                isCalc_deceleration = false;
            }

            //徐々にスピードを上げる
            moveSpeed_now += 0.05f;

            //maxに達したらそこで止める
            if (playerData.isDash)
            {
                if (moveSpeed_now >= MaxDashSpeed)
                {
                    moveSpeed_now = MaxDashSpeed;
                }
            }
            else
            {
                if (moveSpeed_now >= MaxRunSpeed)
                {
                    moveSpeed_now = MaxRunSpeed;
                }
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

        velocity = VScale(moveDirection, moveSpeed_now);
    }


    //重力だけ前フレームのモノを使用
    velocity.y = moveVec_old.y;

    //重力計算
    velocity = Gravity(velocity, playerData);

    //ジャンプ計算
    if (playerData.isRunWall)
    {
        velocity = Run_Wall(velocity, playerData);
    }
    else
    {
        velocity = Jump(velocity, animNumber_Now, playerData);
    }

    //moveVecを保存
    moveVec_old = velocity;

    return velocity;
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
        if (playerData.isJumpSecond)
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

    if (playerData.IsPushRT)
    {
        //特定のフレームまで移動しない
        if (playTime_anim >= 10.0f)
        {
            //※徐々にスピードを下げていく
            //途中で向きがあまり変更できないようにする
            move = moveDirection;
        }

        if (playTime_anim >= 10.0f ||
            animNumber_Now != animNum::quickRoll)
        {
            move = VScale(moveVec, rollMoveSpeed_max);
            moveSpeed_now = MaxRunSpeed;
        }
    }

    return move;
}

VECTOR PlayerCalculation::Run_Wall(
    const VECTOR& moveVec,
    const PlayerData& playerData)
{
    VECTOR move = moveVec;

    //一度だけジャンプ力を付与
    if (isJumpPower_add)
    {
        isRunWall_Stop = false;
        wallRun_stopTime = 0.0f;
        move.y = 0.0f;
        move.y += jumpPower_now;
        isJumpPower_add = false;
    }
    
    //ジャンプ力が0になったら少し留まる
    if (move.y <= 0.0f && !isRunWall_Stop)
    {
        isRunWall_Stop = true;
        move.y = 0.0f;
    }

    //少しの間留まる
    if (isRunWall_Stop)
    {
        if (wallRun_stopTime <= wallRun_stopTime_max)
        {
            wallRun_stopTime++;
            move.y = 0.0f;
        }
        
    }

    return move;
}

void PlayerCalculation::ObstacleCheck(
    const VECTOR& moveDirection,
    const VECTOR& playerPosition,
    const float radius)
{
    const float reverseScale = -1.0f;
    const VECTOR reverseMoveDirection = VScale(moveDirection, reverseScale);
    const auto collisionObjects = WorldSubSystem::GetInstance().GetSubSystem<CollisionObjectManager>()->GetCollisionObjects();
    const VECTOR wallContactPlayerPosition = VAdd(playerPosition, VScale(moveDirection, radius));

    for (const auto& collisionObject : collisionObjects)
    {
        
    }
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
    //DebugDrawer::Instance().InformationInput_sphere(nearestResult.nearestPoint, 2.0f, GetColor(0, 255, 255));
    return newPos;
}

/// <summary>
/// 掴んでいる場所の方向を計算
/// </summary>
/// <param name="hangingPoly"></param>
/// <param name="centerPos"></param>
/// <returns></returns>
VECTOR PlayerCalculation::HangingDirection( const VECTOR& centerPos)
{
    //射影ベクトル
    VECTOR a = Calculation::ProjectionDirection(centerPos, nearestResult.linePos_start, nearestResult.linePos_end);

    VECTOR direction = VSub(a, centerPos);
    direction.y = 0.0f;

    direction = VNorm(direction);

    return direction;
}

/// <summary>
/// 登り
/// </summary>
/// <param name="mapHandle"></param>
VECTOR PlayerCalculation::HangToCrouchMove(
    const std::vector<std::weak_ptr<BaseObject>>& fieldObjects,
    const std::weak_ptr<Player> player)
{
    //////////////////////////////////
    //  コード整理しろ！
    /////////////////////////////////

    const auto& actualPlayer = player.lock();

    VECTOR velocity = VGet(0.0f, 0.0f, 0.0f);

    //指定のフレームまでは手に合わせて座標を更新
    if (actualPlayer->GetNowAnimState().PlayTime_anim <= 22.0f)
    {
        VECTOR addPos = HangingPosition();

        velocity = addPos;
    }
    //
    else
    {
        //上り終わった後に少しずつ前進する
        velocity = VScale(actualPlayer->GetNowMoveDirection(), 0.35f);

        //胸の位置が床の位置をすぎたら足もとを基準に床との衝突判定をする
        VECTOR chestPos = MV1GetFramePosition(actualPlayer->GetModelHandle(), 6);

        VECTOR nowPos = chestPos;

        VECTOR newPos = VAdd(nowPos, velocity);

        //足のフレーム座標で衝突判定
        VECTOR foot = MV1GetFramePosition(actualPlayer->GetModelHandle(), 167);

        auto result = GroundCollisionCheckHangToCrouch(fieldObjects, nowPos, foot, newPos);

        //playerの座標はフレーム座標を基準にしていないため縦だけずらす
        velocity.y = result.second.y - nowPos.y;
    }

    return velocity;
}

/// <summary>
/// 床との衝突判定処理
/// 登るとき専用
/// </summary>
/// <param name="fieldObjects"></param>
/// <param name="topPos"></param>
/// <param name="foot"></param>
/// <param name="position"></param>
/// <returns></returns>
std::pair<bool, VECTOR> PlayerCalculation::GroundCollisionCheckHangToCrouch(
    const std::vector<std::weak_ptr<BaseObject>>& fieldObjects,
    const VECTOR& topPos,
    const VECTOR& foot,
    const VECTOR& position)
{
    VECTOR bottomPos = VGet(topPos.x, foot.y, topPos.z);
    //ごまかしで少し下にrayを伸ばす
    bottomPos.y -= 5.0f;

    MV1_COLL_RESULT_POLY groundRayPoly;
    VECTOR newPosition = position;

    for (const auto& fieldObject : fieldObjects)
    {
        auto collisionObject = fieldObject.lock();

        //rayが当たっていれば
        isWhenClimbingHitGround = HitCheck::RayHitJudge(collisionObject->GetModelHandle(), -1, topPos, bottomPos, groundRayPoly);

        if (isWhenClimbingHitGround)
        {
            VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);

            //床 - プレイヤーの足元で押し戻し量を計算
            newPlayerPos.y = groundRayPoly.HitPosition.y - foot.y;
            newPosition.y = newPosition.y + newPlayerPos.y;
        }
    }

    //接地しているか
    return std::make_pair(isWhenClimbingHitGround, newPosition);
}


void PlayerCalculation::Reset_move()
{
    moveSpeed_now = 0.0f;
    jumpPower_now = 0.0f;
}

void PlayerCalculation::Reset_run_wall()
{
    wallRun_stopTime = 0.0f;
    isRunWall_Stop = false;
}


