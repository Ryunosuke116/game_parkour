#include <iostream>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Falling_Idle.h"

/// <summary>
/// コンストラクタ
/// </summary>
Falling_Idle::Falling_Idle(int modelHandle) : PlayerStateActionBase(modelHandle)
{
	currentPlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
Falling_Idle::~Falling_Idle()
{
	MV1DetachAnim(modelHandle, currentAttachIndex);
}

/// <summary>
/// アニメーション更新
/// </summary>
void Falling_Idle::MotionUpdate()
{
	//再生時間更新
	currentPlayTime_anim += currentPlayAnimSpeed;

	//総再生時間を超えたらリセット
	if (currentPlayTime_anim >= currentTotalPlayTime_anim)
	{
		currentPlayTime_anim = static_cast<float>(fmod(currentPlayTime_anim, currentTotalPlayTime_anim));
	}

	// 再生時間をセットする
	MV1SetAttachAnimTime(modelHandle, currentAttachIndex, currentPlayTime_anim);
}