#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Input.h"
#include "Quick_Roll.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldAnimState"></param>
/// <param name="nowAnimState"></param>
Quick_Roll::Quick_Roll(int& modelHandle,
	OldAnimState& oldAnimState, NowAnimState& nowAnimState) :
	PlayerStateActionBase(modelHandle, oldAnimState, nowAnimState)
{
	// ３Ｄモデルの０番目のアニメーションをアタッチする
	this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::jump_Over);

	this->nowAnimState.PlayTime_anim = 0.0f;
	this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
Quick_Roll::~Quick_Roll()
{

}

/// <summary>
/// アニメーション更新
/// </summary>
/// <param name="playerData"></param>
/// <returns></returns>
bool Quick_Roll::MotionUpdate(PlayerData& playerData)
{
    float totalTime_anim;
    float flag = false;

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

        //空中にいるとき40fでアニメーションを止める
        if (!playerData.isGround)
        {
          /*  if (nowAnimState.PlayTime_anim <= 33.0f)
            {
                nowAnimState.PlayTime_anim += nowAnimState.PlayAnimSpeed;
       
            }*/
        }
 
            nowAnimState.PlayTime_anim += nowAnimState.PlayAnimSpeed;

        if (nowAnimState.PlayTime_anim >= 40.0f)
        {
            if (!playerData.isGround)
            {
               // playerData.isRoll = false;
                flag = true;
            }
        }

        //総再生時間を超えたらリセット
        if (nowAnimState.PlayTime_anim >= totalTime_anim)
        {
            playerData.isRoll = false;

           // nowAnimState.PlayTime_anim = static_cast<float>(fmod(nowAnimState.PlayTime_anim, totalTime_anim));
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