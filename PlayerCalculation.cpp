#include "Dxlib.h"
#include "Calculation.h"
#include "PlayerData.h"
#include "PlayerCalculation.h"
#include"PlayerState.h"
#include "AnimTime.h"
#include "HitCheck.h"
#include "DebugDrawer.h"
#include "WorldSubSystem.h"
#include "CollisionObjectManager.h"
#include "Player.h"
#include "PlayerManager.h"

PlayerCalculation::PlayerCalculation() :
    gravityPower(0.0f),
    nowJumpPower(0.0f),
    nowMoveSpeed(0.0f),
    nowRollMoveSpeed(0.0f),
    wallRunStopTime(0.0f),
    decelerationSpeed(0.0f),
    isCalcDeceleration(false),
    isSlip_after(false),
    isStopRunWall(false),
    isAddJumpPower(false)
{

}

VECTOR PlayerCalculation::Update(
    const VECTOR& moveDirection, 
    const float playAnimTime,
    const int nowAnimNumber, 
    const PlayerData& playerData)
{
    VECTOR velocity = moveDirection;

    //進むスピードを乗算
      //ロールアクション中はそれに応じた速度
    velocity = Roll(nowAnimNumber,
        velocity,
        moveDirection,
        playAnimTime,
        playerData);
   
    //通常時
    velocity = Move(nowAnimNumber, moveDirection, velocity, playerData);

    //崖掴み中
    if (playerData.isNowHanging)
    {
        velocity = HangingPosition();
    }

    if (playerData.isHangToCrouch)
    {
        velocity = HangToCrouchMove(
            WorldSubSystem::GetInstance().GetSubSystem<CollisionObjectManager>()->GetCollisionObjects(),
            WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>()->GetPlayer());
    }
    else
    {
        isWhenClimbingHitGround = false;
    }

    return velocity;
}

/// <summary>
/// 移動距離計算
/// </summary>
/// <param name="velocity"></param>
/// <param name="moveDirection"></param>
/// <param name="playerData"></param>
/// <returns></returns>
VECTOR PlayerCalculation::Move(const int& nowAnimNumber,
    const VECTOR& moveDirection,
    const VECTOR& velocity, 
    const PlayerData& playerData)
{
    if (playerData.isHanging) return velocity;

    VECTOR newVelocity = velocity;

    //ロールアクション中でなければスピード調整
    if (!playerData.isRoll)
    {
        newVelocity = AdjustmentVelocity(playerData, moveDirection);
    }

    //床に沿うように移動するために投影する
    newVelocity = CalcProjectionVelocity(playerData, newVelocity);

    //重力計算
    newVelocity = GravityUpdate(newVelocity, playerData);

    //壁を走るときに一度だけジャンプ力を加算
    if (playerData.isRunWall)
    {
        newVelocity = RunWall(newVelocity, playerData);
    }
    //ジャンプ計算
    else
    {
        newVelocity = Jump(newVelocity, nowAnimNumber, playerData);
    }

    return newVelocity;
}

/// <summary>
/// ジャンプ力計算
/// </summary>
/// <param name="velocity"></param>
/// <param name="nowAnimNumber"></param>
/// <param name="playerData"></param>
/// <returns></returns>
VECTOR PlayerCalculation::Jump(const VECTOR& velocity,const int& nowAnimNumber,
    const PlayerData& playerData)
{
    VECTOR newVelocity = velocity;

    if (isAddJumpPower)
    {
        if (playerData.isSecondJump)
        {
            newVelocity.y = 0.0f;
            gravityPower = 0.0f;
        }
        newVelocity.y += nowJumpPower;
        gravityPower += nowJumpPower;
        isAddJumpPower = false;
    }

    return newVelocity;
}

/// <summary>
/// 重力計算
/// </summary>
/// <param name="velocity"></param>
/// <param name="playerData"></param>
VECTOR PlayerCalculation::GravityUpdate(const VECTOR& velocity,
    const PlayerData& playerData)
{
    VECTOR newVelocity = velocity;

    //接地している場合は重力をリセット
    if (playerData.isGround)
    {
        gravityPower = 0.0f;
    }

    newVelocity.y += gravityPower;

    newVelocity.y += kGravity;
    gravityPower += kGravity;

    return newVelocity;
}

/// <summary>
/// ロール距離計算
/// </summary>
/// <param name="velocity"></param>
/// <param name="moveDirection"></param>
/// <param name="playAnimTime"></param>
/// <param name="playerData"></param>
/// <returns></returns>
VECTOR PlayerCalculation::Roll(const int& nowAnimNumber, 
    const VECTOR& velocity, 
    const VECTOR& moveDirection,
    const float playAnimTime,
    const PlayerData& playerData)
{
    VECTOR newVelocity = velocity;

    if (playerData.isRoll)
    {
        const float kMaxPlayAnimTime = 10.0f;

        //特定のフレームまで移動しない
        if (playAnimTime >= kMaxPlayAnimTime)
        {
            //※徐々にスピードを下げていく
            //途中で向きがあまり変更できないようにする
            newVelocity = moveDirection;
        }

        if (playAnimTime >= kMaxPlayAnimTime ||
            nowAnimNumber != animNum::quickRoll)
        {
            newVelocity = VScale(velocity, kMaxRollMoveSpeed);
            nowMoveSpeed = kMaxRunSpeed;
        }
    }

    return newVelocity;
}

VECTOR PlayerCalculation::RunWall(
    const VECTOR& velocity,
    const PlayerData& playerData)
{
    VECTOR newVelocity = velocity;

    //一度だけジャンプ力を付与
    if (isAddJumpPower)
    {
        isStopRunWall = false;
        wallRunStopTime = 0.0f;
        newVelocity.y = 0.0f;
        gravityPower = 0.0f;

        newVelocity.y += nowJumpPower;
        gravityPower += nowJumpPower;
        isAddJumpPower = false;
    }
    
    //ジャンプ力が0になったら少し留まる
    if (newVelocity.y <= 0.0f && !isStopRunWall)
    {
        isStopRunWall = true;
        newVelocity.y = 0.0f;
        gravityPower = 0.0f;
    }

    //少しの間留まる
    if (isStopRunWall)
    {
        if (wallRunStopTime <= kWallRunMaxStopTime)
        {
            wallRunStopTime++;
            newVelocity.y = 0.0f;
            gravityPower = 0.0f;
        }
    }

    return newVelocity;
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
/// <param name="leftHandPos"></param>
/// <param name="RightHandPos"></param>
/// <param name="nearestPoint"></param>
/// <returns></returns>
VECTOR PlayerCalculation::HangingPosition()
{
    const float kHalfScale = 0.5f;

    VECTOR centerPos = VAdd(leftHandPos, RightHandPos);
    centerPos = VScale(centerPos, kHalfScale);
    
    VECTOR newPos = VSub(nearestResult.nearestPoint, centerPos);
    return newPos;
}

/// <summary>
/// 掴んでいる場所の方向を計算
/// </summary>
/// <param name="hangingPoly"></param>
/// <param name="centerPos"></param>
/// <returns></returns>
VECTOR PlayerCalculation::HangingDirection(const VECTOR& centerPos)
{
    //射影ベクトル
    VECTOR projectionDirection = 
        Calculation::ProjectionDirection(centerPos,
        nearestResult.startLinePos,
        nearestResult.endLinePos);

    VECTOR direction = VSub(projectionDirection, centerPos);
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
    const std::shared_ptr<Player> player)
{
    VECTOR velocity = VGet(0.0f, 0.0f, 0.0f);

    //指定のフレームまでは手の位置に合わせて座標を更新
    if (player->GetNowAnimState().playAnimTime <= 22.0f)
    {
        VECTOR addPos = HangingPosition();

        velocity = addPos;
    }
    else
    {
        const float kScaleVelocity = 0.35f;
        const int kChestBoneNumber = MV1SearchFrame(player->GetModelHandle(), 
            "mixamorig:Spine1");
        const int kFootBoneNumber = MV1SearchFrame(player->GetModelHandle(),
            "mixamorig:RightToeBase");

        //上り終わった後に少しずつ前進する
        velocity = VScale(player->GetNowMoveDirection(), kScaleVelocity);

        //胸の位置が床の位置をすぎたら足もとを基準に床との衝突判定をする
        VECTOR chestPos = MV1GetFramePosition(player->GetModelHandle(), kChestBoneNumber);

        VECTOR nowPos = chestPos;

        VECTOR newPos = VAdd(nowPos, velocity);

        //足のフレーム座標で衝突判定
        VECTOR footPos = MV1GetFramePosition(player->GetModelHandle(), kFootBoneNumber);
        footPos = VGet(chestPos.x, footPos.y, chestPos.z);

        auto result = GroundCollisionCheckHangToCrouch(
            fieldObjects,
            nowPos, 
            footPos, 
            newPos);

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
    const VECTOR& startRayPos,
    const VECTOR& endRayPos,
    const VECTOR& position)
{
    const int kFrameIndex = -1;
    VECTOR bottomPos = VGet(startRayPos.x, endRayPos.y, startRayPos.z);

    MV1_COLL_RESULT_POLY groundRayPoly;
    VECTOR newPosition = position;

    for (const auto& fieldObject : fieldObjects)
    {
        auto collisionObject = fieldObject.lock();

        //rayが当たっていれば
        isWhenClimbingHitGround = HitCheck::RayHitJudge(
            collisionObject->GetModelHandle(),
            kFrameIndex,
            startRayPos,
            bottomPos,
            groundRayPoly);

        if (isWhenClimbingHitGround)
        {
            VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);

            //床 - プレイヤーの足元で押し戻し量を計算
            newPlayerPos.y = groundRayPoly.HitPosition.y - endRayPos.y;
            newPosition.y = newPosition.y + newPlayerPos.y;
        }
    }

    //接地しているか
    return std::make_pair(isWhenClimbingHitGround, newPosition);
}

VECTOR PlayerCalculation::CalcProjectionVelocity(
    const PlayerData& playerData,
    const VECTOR& velocity)
{
    bool isCalc = playerData.isGround &&
        VSize(velocity) != 0 &&
        !playerData.isJump;

    //床に沿うように移動する
    if (isCalc)
    {
        VECTOR newVelocity = velocity;
        //移動量を計算
        float size = VSize(newVelocity);

        //方向ベクトルを計算
        newVelocity = Calculation::Projection(nowGroundRayPoly.Normal, newVelocity);

        //移動量を再計算
        newVelocity = VScale(newVelocity, size);

        return newVelocity;
    }

    return velocity;
}

VECTOR PlayerCalculation::AdjustmentVelocity(
    const PlayerData& playerData, 
    const VECTOR& moveDirection)
{
    //動いている場合移動スピードを徐々に上げる
    if (playerData.isMove)
    {
        nowMoveSpeed = SpeedUp();
    }
    else
    {
        if (playerData.isIdle)
        {
            nowMoveSpeed = 0.0f;
        }
        else
        {
            nowMoveSpeed = SpeedDown(playerData);
        }
    }

    return VScale(moveDirection, nowMoveSpeed);
}

float PlayerCalculation::SpeedUp()
{
    const float kAddMoveSpeed = 0.05f;
    float newMoveSpeed = nowMoveSpeed;

    //減速スピード計算を一度行っていたらfalseに
    if (isCalcDeceleration)
    {
        isCalcDeceleration = false;
    }

    //徐々にスピードを上げる
    newMoveSpeed += kAddMoveSpeed;

    //maxに達したらそこで止める
    if (newMoveSpeed >= kMaxRunSpeed)
    {
        newMoveSpeed = kMaxRunSpeed;
    }

    return newMoveSpeed;
}

float PlayerCalculation::SpeedDown(const PlayerData& playerData)
{
    float newMoveSpeed = nowMoveSpeed;

    //接地しているときに止まったらすぐ止まる
    if (playerData.isGround)
    {
        //アニメーションフレームに合わせて減速
        if (!isCalcDeceleration)
        {
            const float kStopFrame = 10.0f;
            decelerationSpeed = newMoveSpeed / kStopFrame;
            isCalcDeceleration = true;
        }

        newMoveSpeed -= decelerationSpeed;
    }
    //空中にいるとき
    else
    {
        const float kDownSpeed = 0.02f;
        //徐々に下げる
        newMoveSpeed -= kDownSpeed;
    }

    //0未満の場合0にする
    if (newMoveSpeed < 0.0f)
    {
        newMoveSpeed = 0.0f;
    }

    return newMoveSpeed;
}

void PlayerCalculation::ResetMove()
{
    nowMoveSpeed = 0.0f;
    nowJumpPower = 0.0f;
}

void PlayerCalculation::ResetWallRun()
{
    wallRunStopTime = 0.0f;
    isStopRunWall = false;
}


