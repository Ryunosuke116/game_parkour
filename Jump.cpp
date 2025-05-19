#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Input.h"
#include "Jump.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Jump::Jump(int& modelHandle,
    OldAnimState& oldAnimState, NowAnimState& nowAnimState) :
    PlayerStateActionBase(modelHandle, oldAnimState,nowAnimState)
{
    input = std::make_shared<Input>();

    // ３Ｄモデルの０番目のアニメーションをアタッチする
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::jump);

    this->nowAnimState.PlayTime_anim = 5.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
    this->nowAnimState.TotalPlayTime_anim = MV1GetAttachAnimTotalTime(modelHandle, this->nowAnimState.AttachIndex);
    isPush = false;
}

/// <summary>
/// デストラクタ
/// </summary>
Jump::~Jump()
{
  //  MV1DetachAnim(modelHandle, this->nowAnimState.AttachIndex);
}

/// <summary>
/// アニメーション更新 
/// </summary>
/// <returns></returns>
bool Jump::MotionUpdate(PlayerData& playerData)
{
    input->Update();

    bool flag = false;

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
        //二段ジャンプしたらプレイタイムリセット
        if (!isPush)
        {
            if (playerData.isJump_second)
            {
                nowAnimState.PlayTime_anim = 5.0f;
                isPush = true;
            }
        }

        // アタッチしたアニメーションの総再生時間を取得する
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.AttachIndex);

        //再生時間更新
        nowAnimState.PlayTime_anim += nowAnimState.PlayAnimSpeed;

        //総再生時間を超えたらリセット
        if (nowAnimState.PlayTime_anim >= totalTime_anim)
        {
            flag = true;
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
        oldAnimState.PlayTime_anim += oldAnimState.PlayAnimSpeed;

        // 再生時間が総時間に到達していたら再生時間をループさせる
        if (oldAnimState.PlayTime_anim > totalTime_anim)
        {
            oldAnimState.PlayTime_anim = static_cast<float>(fmod(oldAnimState.PlayTime_anim, totalTime_anim));
        }

        // 変更した再生時間をモデルに反映させる
        MV1SetAttachAnimTime(modelHandle, oldAnimState.AttachIndex, oldAnimState.PlayTime_anim);

        // アニメーション２のモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.AttachIndex, 1.0f - animBlendRate);
    }

    return flag;
}