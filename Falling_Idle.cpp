#include <iostream>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Falling_Idle.h"

/// <summary>
/// コンストラクタ
/// </summary>
Falling_Idle::Falling_Idle(int& modelHandle,
	OldAnimState& oldAnimState, NowAnimState& nowAnimState) :
	PlayerStateActionBase(modelHandle, oldAnimState,nowAnimState)
{
	this->nowAnimState.PlayAnimSpeed = playAnimSpeed;

	// ３Ｄモデルの０番目のアニメーションをアタッチする
	this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::Falling_Idle);

	this->nowAnimState.PlayTime_anim = 0.0f;
	this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
	animBlendRate = 1.0f;

}

/// <summary>
/// デストラクタ
/// </summary>
Falling_Idle::~Falling_Idle()
{
	//MV1DetachAnim(modelHandle, nowAnimState.AttachIndex);
}

/// <summary>
/// アニメーション更新
/// </summary>
bool Falling_Idle::MotionUpdate(PlayerData& playerData)
{
	// アタッチしたアニメーションの総再生時間を取得する
	float totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.AttachIndex);

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

	return false;
}