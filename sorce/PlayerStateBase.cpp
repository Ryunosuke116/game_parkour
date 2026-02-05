#include "Common.h"
#include <memory>
#include <vector>
#include "PadInput.h"
#include "PlayerData.h"
#include "PlayerStateBase.h"
#include "Player.h"
#include "DebugDrawer.h"
#include "GameInstanceSubSystem.h"
#include "WorldSubSystem.h"
#include "CollisionObjectManager.h"
#include "EffectManager.h"
#include "HitCheck.h"
#include "PlayerCalculation.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="prevAttachIndex"></param>
/// <param name="oldAnimState"></param>
/// <param name="nowAnimState"></param>
PlayerStateBase::PlayerStateBase(const int modelHandle):
    modelHandle(-1),
    animOldNumber(-1),
    animBlendRate(0.0f),
    isChangeState(false),
    isChoiceCommand(false),
    isPush(false),
    moveDirection(VGet(0.0f, 0.0f, 0.0f)),
    nowAnimState({NULL}),
    oldAnimState({NULL})
{
    this->modelHandle = modelHandle;
    nowAnimState.attachIndex = -1;
    oldAnimState.attachIndex = -1;
}

void PlayerStateBase::Enter(const AnimState& oldAnimState, 
    const AnimState& nowAnimState)
{
    isChangeState = false;

    //いままで情報をprevに保存
    this->oldAnimState.attachIndex = oldAnimState.attachIndex;

    if (this->oldAnimState.attachIndex != -1)
    {
        MV1DetachAnim(this->modelHandle, this->oldAnimState.attachIndex);
        this->oldAnimState.attachIndex = -1;
    }

    //いままで情報をprevに保存
    this->oldAnimState.attachIndex = nowAnimState.attachIndex;
    this->oldAnimState.playAnimTime = nowAnimState.playAnimTime;
    this->oldAnimState.playAnimSpeed = nowAnimState.playAnimSpeed;

    // ブレンド率はPrevが有効ではない場合は１．０ｆ( 現在モーションが１００％の状態 )にする
    animBlendRate = this->oldAnimState.attachIndex == -1 ? 1.0f : 0.0f;
}

void PlayerStateBase::Initialize(const int modelHandle,
    const int changeNum,
    Player& player)
{
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    this->nowAnimState.attachIndex = MV1AttachAnim(this->modelHandle, changeNum);

    this->nowAnimState.playAnimTime = 0.0f;
    this->nowAnimState.totalPlayAnimTime = MV1GetAttachAnimTotalTime(this->modelHandle, this->nowAnimState.attachIndex);
}

/// <summary>
/// アニメーション更新
/// </summary>
/// <returns></returns>
bool PlayerStateBase::MotionUpdate(PlayerData& playerData)
{
    float totalPlayAnimTime = 0.0f;

    // ブレンド率が１以下の場合は１に近づける
    if (animBlendRate < 1.0f)
    {
        animBlendRate += kAnimBlendSpeed;
        if (animBlendRate > 1.0f)
        {
            animBlendRate = 1.0f;
        }
    }

    if (nowAnimState.attachIndex != -1)
    {
        //再生時間更新
        nowAnimState.playAnimTime += nowAnimState.playAnimSpeed;

        //総再生時間を超えたらリセット
        if (nowAnimState.playAnimTime >= nowAnimState.totalPlayAnimTime)
        {
            nowAnimState.playAnimTime = static_cast<float>(fmod(nowAnimState.playAnimTime, nowAnimState.totalPlayAnimTime));
        }

        // 再生時間をセットする
        MV1SetAttachAnimTime(modelHandle, nowAnimState.attachIndex, nowAnimState.playAnimTime);

        //アニメーションのモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.attachIndex, animBlendRate);
    }

    //再生しているアニメーション２の処理
    if (oldAnimState.attachIndex != -1)
    {
        // アニメーションの総時間を取得
        totalPlayAnimTime = MV1GetAttachAnimTotalTime(modelHandle, oldAnimState.attachIndex);

        // 変更した再生時間をモデルに反映させる
        MV1SetAttachAnimTime(modelHandle, oldAnimState.attachIndex, oldAnimState.playAnimTime);

        // アニメーション２のモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.attachIndex, 1.0f - animBlendRate);
    }

    return false;
}


void PlayerStateBase::Draw()
{
    DebugDrawer::GetInstance().InformationInputStringInt("nowAttachIndex %d\n", nowAnimState.attachIndex);
    DebugDrawer::GetInstance().InformationInputStringInt("oldAttachIndex %d\n", oldAnimState.attachIndex);

    DebugDrawer::GetInstance().InformationInputStringFloat("nowPlayTime_anim %f\n", nowAnimState.playAnimTime);
    DebugDrawer::GetInstance().InformationInputStringFloat("oldPlayTime_anim %f\n", oldAnimState.playAnimTime);
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="input"></param>
/// <param name="velocity"></param>
VECTOR PlayerStateBase::Move(
    const VECTOR& cameraDirection,
    PlayerData& playerData)
{
    moveDirection = VGet(0.0f, 0.0f, 0.0f);

    playerData.isMove   = false;
    VECTOR rightMove    = VCross(cameraDirection, VGet(0.0f, 1.0f, 0.0f));

    //正規化
    rightMove       = VNorm(rightMove);
    VECTOR upMove   = VNorm(cameraDirection);

    upMove.y    = 0.0f;
    rightMove.y = 0.0f;

    //パッド or arrowキーの入力方向で計算
    moveDirection = VAdd(VScale(rightMove, -PadInput::GetJoyPadXLeft()),
        VScale(upMove, -PadInput::GetJoyPadYLeft()));

    //0でなければ正規化
    if (VSize(moveDirection) != 0)
    {
        moveDirection = VNorm(moveDirection);
    }

    //入力されてなければ動いてない
    if (VSize(moveDirection) == 0)
    {
        playerData.isMove = false;
    }
    else
    {
        playerData.isMove = true;
    }

    //必ず正規化されたものか0を返す
    return moveDirection;

}

/// <summary>
/// ロールアクション入力
/// </summary>
void PlayerStateBase::RollMove(PlayerData& playerData)
{
    //他の状態に遷移することが決まっている場合処理を行わない
    if (isChangeState)
    {
        return;
    }

    if (PadInput::IsPushRT() && 
        !playerData.isRoll && 
        !playerData.isUseRoll)
    {
        playerData.isRoll       = true;
        playerData.isUseRoll    = true;
        playerData.isAllJump    = false;
        playerData.isSecondJump = false;
        isChangeState           = true;
        isChoiceCommand         = true;
    }
}

/// <summary>
/// ジャンプ
/// </summary>
void PlayerStateBase::JumpMove(PlayerData& playerData, 
    Player& player)
{
    //他の状態に遷移することが決まっている場合処理を行わない
    if (isChangeState)
    {
        return;
    }

    bool isFirstJump = !player.playerCalculation->GetIsAddJumpPower() &&
        !isPush &&
        !playerData.isFirstJump;
    
    bool isSecondJump = playerData.isFirstJump &&
        !isPush &&
        !playerData.isSecondJump;

    if (PadInput::isJump() && !playerData.isAllJump)
    {
        //ジャンプ
        if (isFirstJump)
        {
            isChangeState           = true;
            playerData.isJump       = true;
            playerData.isFirstJump  = true;
            isPush                  = true;
            isChoiceCommand         = true;
            player.playerCalculation->ChangeTrueIsAddJumpPower();
            player.playerCalculation->SetJumpPower();
        }
        //二段ジャンプ
        else if (isSecondJump)
        {
            const auto effectManager = GameInstanceSubSystem::GetInstance().GetSubSystem<EffectManager>().lock();
            effectManager->PlayEffect("jump");
            effectManager->SetPosition(player.GetPosition(), "jump");

            if (!playerData.isJump)
            {
                isChangeState       = true;
                playerData.isJump   = true;
            }

            isPush                  = true;
            playerData.isSecondJump = true;
            playerData.isAllJump    = true;
            isChoiceCommand         = true;
            player.playerCalculation->ChangeTrueIsAddJumpPower();
            player.playerCalculation->SetSecondJumpPower();

            if (playerData.isMove)
            {
                nowAnimState.playAnimTime = kInitMoveJumpPlayTime;
            }
            else
            {
                nowAnimState.playAnimTime = kInitNormalJumpPlayTime;
            }
        }
    }
    else
    {
        isPush = false;
    }
}

/// <summary>
/// ウォールランするか
/// </summary>
/// <param name="playerData"></param>
/// <param name="player"></param>
bool PlayerStateBase::WallRunMove(PlayerData& playerData, 
    Player& player,
    const std::shared_ptr<BaseObject>& collisionObject)
{
    //既に行動が決まっていたら抜ける
    if (isChoiceCommand)
    {
        return false;
    }

    const float kEntryDegreeWallRun = 50.0f;
    const float kAddRayEndPosition  = 5.0f;
    const int kFrameIndex           = -1;

    MV1_COLL_RESULT_POLY hitPoly;
    VECTOR addPos           = VScale(player.GetFaceDirection(), kAddRayEndPosition);
    addPos.y                = 0.0f;
    VECTOR rayEndPosition   = VAdd(player.GetPositionData().rayTopPosition, addPos);

    //壁を走れるか
    bool isUseWallRun = HitCheck::RayHitJudge(collisionObject->GetModelHandle(),
        kFrameIndex,
        player.GetPositionData().rayTopPosition,
        rayEndPosition,
        hitPoly) &&
        playerData.isUseWallJump &&
        playerData.isMove;

    if (isUseWallRun)
    {
        VECTOR hitWallNormal = hitPoly.Normal;

        //スティックが即座に反対方向に向いた場合slipをtrue
        //radian計算
        float radianWall            = atan2f(-hitWallNormal.x, -hitWallNormal.z);
        float radianFaceDirection   = atan2f(player.GetFaceDirection().x,
            player.GetFaceDirection().z);

        //度数計算
        float degreeWall            = abs(Calculation::RadToDeg(radianWall));
        float degreeFaceDirection   = abs(Calculation::RadToDeg(radianFaceDirection));

        //スティック入力と壁の角度の差を求める
        float degreePadWallDifference = degreeFaceDirection - degreeWall;

        //壁の法線ベクトルを利用して壁走りするかどうか
        if (abs(degreePadWallDifference) <= kEntryDegreeWallRun)
        {
            const float kInversionScale = -1.0f;

            //ロールアクションとジャンプをできないように
            playerData.isRunWall     = true;
            playerData.isRun         = true;
            playerData.isUseWallJump = false;
            playerData.isSecondJump  = false;
            playerData.isAllJump     = false;
            isChangeState            = true;
            isChoiceCommand          = true;

            player.playerCalculation->ChangeTrueIsAddJumpPower();
            player.playerCalculation->SetJumpPower();
            player.playerCalculation->SetWallRunGravity(hitWallNormal);
            player.SetNowMoveDirection(VScale(hitWallNormal, kInversionScale));
            player.SetFaceDirection(VScale(hitWallNormal, kInversionScale));
            player.SetRotateX(kRunWallRotateX);
        }
    }

    return true;
}

/// <summary>
/// ジャンプ状況リセット
/// </summary>
void PlayerStateBase::ResetIsJumps(PlayerData& playerData)
{
    if (playerData.isGround)
    {
        playerData.isFirstJump = false;
        playerData.isSecondJump = false;
        playerData.isAllJump = false;
    }
}

/// <summary>
/// アタッチするアニメーションを変更
/// </summary>
/// <param name="animNum"></param>
void PlayerStateBase::SwitchingAnimation(const int animNum)
{
    //古い情報を削除
    if (this->oldAnimState.attachIndex != -1)
    {
        MV1DetachAnim(modelHandle, this->oldAnimState.attachIndex);
        this->oldAnimState.attachIndex = -1;
    }

    ////いままで情報をprevに保存
    this->oldAnimState.attachIndex = nowAnimState.attachIndex;
    this->oldAnimState.playAnimTime = nowAnimState.playAnimTime;
    animBlendRate = 0.0f;

    //アニメーションをアタッチ
    this->nowAnimState.attachIndex = MV1AttachAnim(modelHandle, animNum);
    nowAnimState.playAnimTime = 0.0f;
}

void PlayerStateBase::Exit(PlayerData& playerData)
{
    isChoiceCommand = false;
}