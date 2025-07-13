#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Run_To_Stop.h"
#include "AnimTime.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Run_To_Stop::Run_To_Stop(int& modelHandle,
    AnimState& oldAnimState, AnimState& nowAnimState) :
    PlayerStateActionBase(modelHandle, oldAnimState, nowAnimState)
{
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::run_To_Stop);

    this->nowAnimState.PlayTime_anim = 0.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
Run_To_Stop::~Run_To_Stop()
{
    //  MV1DetachAnim(modelHandle, this->nowAnimState.AttachIndex);
}

/// <summary>
/// アニメーション更新
/// </summary>
/// <returns></returns>
bool Run_To_Stop::MotionUpdate(PlayerData& playerData)
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
            playerData.isStopRun = false;
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

        // 変更した再生時間をモデルに反映させる
        MV1SetAttachAnimTime(modelHandle, oldAnimState.AttachIndex, oldAnimState.PlayTime_anim);

        // アニメーション２のモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.AttachIndex, 1.0f - animBlendRate);
    }

    if (!playerData.isGround)
    {
        return true;
    }

    return false;
}