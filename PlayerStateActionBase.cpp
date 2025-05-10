#include <iostream>
#include <vector>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
<<<<<<< HEAD
PlayerStateActionBase::PlayerStateActionBase(int modelHandle, int& prevAttachIndex, NowAnimState nowAnimState):
    modelHandle(-1),
    animBlendRate(0.0f),
    prevAttachIndex(-1),
    prevPlayTime_anim(0.0f),
    prevTotalPlayTime_anim(0.0f),
    prevPlayAnimSpeed(0.0f)
{

    this->nowAnimState.currentAttachIndex = 0;
    this->nowAnimState.currentPlayAnimSpeed = 0.0f;
    this->nowAnimState.currentPlayTime_anim = 0.0f;
    this->nowAnimState.currentTotalPlayTime_anim = 0.0f;

=======
PlayerStateActionBase::PlayerStateActionBase(int modelHandle)
{
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c
	this->modelHandle = modelHandle;

    if (prevAttachIndex != -1)
    {
        MV1DetachAnim(modelHandle, prevAttachIndex);
        prevAttachIndex = -1;
    }

    //いままで情報をprevに保存
<<<<<<< HEAD
    this->prevAttachIndex = nowAnimState.currentAttachIndex;
    prevPlayTime_anim = nowAnimState.currentPlayTime_anim;
    prevPlayAnimSpeed = nowAnimState.currentPlayAnimSpeed;

    // 再生時間の初期化
    //nowAnimState.currentPlayTime_anim = 0.0f;

    // アタッチしたアニメーションの総再生時間を取得する
   // this->nowAnimState.currentTotalPlayTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.currentAttachIndex);
=======
    prevAttachIndex = currentAttachIndex;
    prevPlayTime_anim = currentPlayTime_anim;
    prevPlayAnimSpeed = currentPlayAnimSpeed;

    // ３Ｄモデルの０番目のアニメーションをアタッチする
    currentAttachIndex = MV1AttachAnim(modelHandle, animNum::Idle);

    // 再生時間の初期化
    currentPlayTime_anim = 0.0f;

    // アタッチしたアニメーションの総再生時間を取得する
    currentTotalPlayTime_anim = MV1GetAttachAnimTotalTime(modelHandle, currentAttachIndex);
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c

    // ブレンド率はPrevが有効ではない場合は１．０ｆ( 現在モーションが１００％の状態 )にする
    animBlendRate = prevAttachIndex == -1 ? 1.0f : 0.0f;
}

void PlayerStateActionBase::MotionUpdate()
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

<<<<<<< HEAD
    if (nowAnimState.currentAttachIndex != -1)
    {
        // アタッチしたアニメーションの総再生時間を取得する
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.currentAttachIndex);

        //再生時間更新
        nowAnimState.currentPlayTime_anim += nowAnimState.currentPlayAnimSpeed;

        //総再生時間を超えたらリセット
        if (nowAnimState.currentPlayTime_anim >= totalTime_anim)
        {
            nowAnimState.currentPlayTime_anim = static_cast<float>(fmod(nowAnimState.currentPlayTime_anim, totalTime_anim));
        }

        // 再生時間をセットする
        MV1SetAttachAnimTime(modelHandle, nowAnimState.currentAttachIndex, nowAnimState.currentPlayTime_anim);

        //アニメーションのモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.currentAttachIndex, animBlendRate);
=======
    if (currentAttachIndex != -1)
    {
        // アタッチしたアニメーションの総再生時間を取得する
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, currentAttachIndex);

        //再生時間更新
        currentPlayTime_anim += currentPlayAnimSpeed;

        //総再生時間を超えたらリセット
        if (currentPlayTime_anim >= totalTime_anim)
        {
            currentPlayTime_anim = static_cast<float>(fmod(currentPlayTime_anim, totalTime_anim));
        }

        // 再生時間をセットする
        MV1SetAttachAnimTime(modelHandle, currentAttachIndex, currentPlayTime_anim);

        //アニメーションのモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, currentAttachIndex, animBlendRate);
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c
    }


    //再生しているアニメーション２の処理
    if (prevAttachIndex != -1)
    {
        // アニメーションの総時間を取得
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, prevAttachIndex);

        // 再生時間を進める
        prevPlayTime_anim += prevPlayAnimSpeed;

        // 再生時間が総時間に到達していたら再生時間をループさせる
        if (prevPlayTime_anim > totalTime_anim)
        {
            prevPlayTime_anim = static_cast<float>(fmod(prevPlayTime_anim, totalTime_anim));
        }

        // 変更した再生時間をモデルに反映させる
        MV1SetAttachAnimTime(modelHandle, prevAttachIndex, prevPlayTime_anim);

        // アニメーション２のモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, prevAttachIndex, 1.0f - animBlendRate);
    }
}

