#include "common.h"
#include <memory>
#include <vector>
#include "PadInput.h"
#include "PlayerData.h"
#include "PlayerStateActionBase.h"
#include "Player.h"
#include "DebugDrawer.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="prevAttachIndex"></param>
/// <param name="oldAnimState"></param>
/// <param name="nowAnimState"></param>
PlayerStateActionBase::PlayerStateActionBase(int& modelHandle,
    AnimState& oldAnimState, AnimState& nowAnimState):
    modelHandle(-1),
    animBlendRate(0.0f),
    nowAnimState({NULL}),
    oldAnimState({NULL}),
    isChangeState(false)
{
    this->modelHandle = modelHandle;

    //いままで情報をprevに保存
    this->oldAnimState.AttachIndex = oldAnimState.AttachIndex;

    if (this->oldAnimState.AttachIndex != -1)
    {
        MV1DetachAnim(this->modelHandle, this->oldAnimState.AttachIndex);
        this->oldAnimState.AttachIndex = -1;
    }

    //いままで情報をprevに保存
    this->oldAnimState.AttachIndex = nowAnimState.AttachIndex;
    this->oldAnimState.PlayTime_anim = nowAnimState.PlayTime_anim;
    this->oldAnimState.PlayAnimSpeed = nowAnimState.PlayAnimSpeed;

    // ブレンド率はPrevが有効ではない場合は１．０ｆ( 現在モーションが１００％の状態 )にする
    animBlendRate = this->oldAnimState.AttachIndex == -1 ? 1.0f : 0.0f;
}

/// <summary>
/// アニメーション更新
/// </summary>
/// <returns></returns>
bool PlayerStateActionBase::MotionUpdate(PlayerData& playerData)
{
    float totalTime_anim;

    // ブレンド率が１以下の場合は１に近づける
    if (animBlendRate < 1.0f)
    {
        animBlendRate += AnimBlendSpeed;
        if (animBlendRate > 1.0f)
        {
            animBlendRate = 1.0f;
        }
    }

    if (nowAnimState.AttachIndex != -1)
    {
        // アタッチしたアニメーションの総再生時間を取得する
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.AttachIndex);

        //再生時間更新
        nowAnimState.PlayTime_anim += nowAnimState.PlayAnimSpeed;

        //総再生時間を超えたらリセット
        if (nowAnimState.PlayTime_anim >= totalTime_anim)
        {
            nowAnimState.PlayTime_anim = static_cast<float>(fmod(nowAnimState.PlayTime_anim, totalTime_anim));
        }

        // 再生時間をセットする
        MV1SetAttachAnimTime(modelHandle, nowAnimState.AttachIndex, nowAnimState.PlayTime_anim);

        //アニメーションのモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.AttachIndex, animBlendRate);
    }


    //再生しているアニメーション２の処理
    if (oldAnimState.AttachIndex != -1)
    {
        // アニメーションの総時間を取得
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, oldAnimState.AttachIndex);

        // 再生時間を進める
       // oldAnimState.PlayTime_anim += oldAnimState.PlayAnimSpeed;

        // 再生時間が総時間に到達していたら再生時間をループさせる
        if (oldAnimState.PlayTime_anim > totalTime_anim)
        {
            //oldAnimState.PlayTime_anim = static_cast<float>(fmod(oldAnimState.PlayTime_anim, totalTime_anim));
        }

        // 変更した再生時間をモデルに反映させる
        MV1SetAttachAnimTime(modelHandle, oldAnimState.AttachIndex, oldAnimState.PlayTime_anim);

        // アニメーション２のモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.AttachIndex, 1.0f - animBlendRate);
    }

    return false;
}


bool PlayerStateActionBase::Draw()
{
    DebugDrawer::Instance().InformationInput_string_float("nowAttachIndex %d\n", nowAnimState.AttachIndex);
    DebugDrawer::Instance().InformationInput_string_float("nowPlayTime_anim %f\n", nowAnimState.PlayTime_anim);
    DebugDrawer::Instance().InformationInput_string_float("oldAttachIndex %d\n", oldAnimState.AttachIndex);
    DebugDrawer::Instance().InformationInput_string_float("oldPlayTime_anim %f\n", oldAnimState.PlayTime_anim);
 
    return true;
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="input"></param>
/// <param name="moveVec"></param>
VECTOR PlayerStateActionBase::Move(const VECTOR& cameraDirection, PlayerData& playerData)
{
    moveDirection = VGet(0.0f, 0.0f, 0.0f);
    VECTOR moveDirection_old = VGet(0.0f, 0.0f, 0.0f);

    playerData.isMove = false;
    VECTOR rightMove = VCross(cameraDirection, VGet(0.0f, 1.0f, 0.0f));

    //正規化
    rightMove = VNorm(rightMove);
    VECTOR upMove = VNorm(cameraDirection);

    upMove.y = 0.0f;
    rightMove.y = 0.0f;

    //前フレームのパッドスティック情報
    moveDirection_old = VAdd(VScale(rightMove, -PadInput::GetJoyPad_old_x_left()),
        VScale(upMove, -PadInput::GetJoyPad_old_y_left()));

    //パッド or arrowキーの入力方向で計算
    moveDirection = VAdd(VScale(rightMove, -PadInput::GetJoyPad_x_left()),
        VScale(upMove, -PadInput::GetJoyPad_y_left()));

    //0でなければ正規化
    if (VSize(moveDirection) != 0)
    {
        moveDirection = VNorm(moveDirection);
        
    }
    

    //前フレームと現在のフレームで入力されてなければ動いてない
    if (VSize(moveDirection_old) == 0 &&
        VSize(moveDirection) == 0)
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
void PlayerStateActionBase::RollMove(PlayerData& playerData)
{
    if (PadInput::isRoll() && !playerData.isRoll && !playerData.isUse_Roll)
    {
        playerData.isRoll = true;
        playerData.isUse_Roll = true;
        isChangeState = true;
    }
}

/// <summary>
/// ジャンプ
/// </summary>
void PlayerStateActionBase::JumpMove(PlayerData& playerData, Player& player)
{
    if (PadInput::isJump() && !playerData.isJumpAll)
    {
        //ジャンプ
        if (!player.playerCalculation->GetIsJumpPower_add() &&
            !isPush && !playerData.isJump_first)
        {
            isChangeState = true;
            playerData.isJump = true;
            playerData.isJump_first = true;
            isPush = true;
            player.playerCalculation->ChangeIsJumpPower_add_ture();
            player.playerCalculation->SetJumpPower();
        }
        //二段ジャンプ
        else if (playerData.isJump_first && !isPush &&
            !playerData.isJump_second)
        {
            if (!playerData.isJump)
            {
                isChangeState = true;
                playerData.isJump = true;
            }
            isPush = true;
            playerData.isJump_second = true;
            playerData.isJumpAll = true;
            player.playerCalculation->ChangeIsJumpPower_add_ture();
            player.playerCalculation->SetJumpPower();

            nowAnimState.PlayTime_anim = 5.0f;
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
void PlayerStateActionBase::WallRunMove(PlayerData& playerData, Player& player)
{

    //壁に当たっている場合のみ
    if (playerData.isHitWall && playerData.isUse_wallJump)
    {
        VECTOR hitWall_normal = player.playerCalculation->GetHitWall_normal();
        
        //スティックが即座に反対方向に向いた場合slipをtrue
        //radian計算
        float radian_wall = atan2f(-hitWall_normal.x, -hitWall_normal.z);
        float radian_pad = atan2f(moveDirection.x, moveDirection.z);

        //度数計算
        float degree_pad_now = Calculation::radToDeg(radian_pad);
        float degree_wall = Calculation::radToDeg(radian_wall);

        //スティック入力と壁の角度の差を求める
        float degree_pad_wall_difference = degree_pad_now - degree_wall;

        //壁の法線ベクトルを利用して壁走りするかどうか
        if (abs(degree_pad_wall_difference) <= entryDegree_wallRun)
        {
            //ロールアクションとジャンプをできないように
            playerData.isRun_wall = true;
            playerData.isRun = true;
            playerData.isUse_wallJump = false;
            playerData.isJump_second = false;
            playerData.isJumpAll = false;
            isChangeState = true;

            player.playerCalculation->ChangeIsJumpPower_add_ture();
            player.playerCalculation->SetJumpPower();
            player.SetMoveDirection_now(VScale(hitWall_normal, -1.0f));
            player.SetRotata_x(run_wall_rotate_x);
        }
    }
}

/// <summary>
/// ジャンプ状況リセット
/// </summary>
void PlayerStateActionBase::FlagReset_jump(PlayerData& playerData)
{
    if (playerData.isGround)
    {
        playerData.isJump_first = false;
        playerData.isJump_second = false;
        playerData.isJumpAll = false;
    }
}

void PlayerStateActionBase::SwitchingAnimation(const int& animNum)
{
    //古い情報を削除
    if (this->oldAnimState.AttachIndex != -1)
    {
        MV1DetachAnim(modelHandle, this->oldAnimState.AttachIndex);
        this->oldAnimState.AttachIndex = -1;
    }

    ////いままで情報をprevに保存
    this->oldAnimState.AttachIndex = nowAnimState.AttachIndex;
    this->oldAnimState.PlayTime_anim = nowAnimState.PlayTime_anim;
    animBlendRate = 0.0f;

    //アニメーションをアタッチ
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum);
    nowAnimState.PlayTime_anim = 0.0f;

}

void PlayerStateActionBase::SetOldAnimState()
{
    oldAnimState.AttachIndex = nowAnimState.AttachIndex;
    oldAnimState.PlayAnimSpeed = nowAnimState.PlayAnimSpeed;
    oldAnimState.PlayTime_anim = nowAnimState.PlayTime_anim;
    oldAnimState.TotalPlayTime_anim = nowAnimState.TotalPlayTime_anim;
}

void PlayerStateActionBase::ResetOldAnimState()
{
    oldAnimState.AttachIndex = -1;
    oldAnimState.PlayAnimSpeed = 0.0f;
    oldAnimState.PlayTime_anim = 0.0f;
    oldAnimState.TotalPlayTime_anim = 0.0f;

}

void PlayerStateActionBase::ResetNowAnimState()
{
    nowAnimState.AttachIndex = -1;
    nowAnimState.PlayAnimSpeed = 0.0f;
    nowAnimState.PlayTime_anim = 0.0f;
    nowAnimState.TotalPlayTime_anim = 0.0f;
}