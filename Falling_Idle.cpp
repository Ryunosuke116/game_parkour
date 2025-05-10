#include <iostream>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Falling_Idle.h"

/// <summary>
/// コンストラクタ
/// </summary>
Falling_Idle::Falling_Idle(int modelHandle, int& prevAttachIndex, NowAnimState nowAnimState) :
	PlayerStateActionBase(modelHandle, prevAttachIndex, nowAnimState)
{
	this->nowAnimState.currentPlayAnimSpeed = playAnimSpeed;

	//古い情報がある場合は削除
	if (prevAttachIndex != -1)
	{
		MV1DetachAnim(modelHandle, prevAttachIndex);
		prevAttachIndex = -1;
	}

	//いままで情報をprevに保存
	prevAttachIndex = this->nowAnimState.currentAttachIndex;
	prevPlayTime_anim = this->nowAnimState.currentPlayTime_anim;
	prevPlayAnimSpeed = this->nowAnimState.currentPlayAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
Falling_Idle::~Falling_Idle()
{
	MV1DetachAnim(modelHandle, nowAnimState.currentAttachIndex);
}

/// <summary>
/// アニメーション更新
/// </summary>
void Falling_Idle::MotionUpdate()
{
	//再生時間更新
	nowAnimState.currentPlayTime_anim += nowAnimState.currentPlayAnimSpeed;

	//総再生時間を超えたらリセット
	if (nowAnimState.currentPlayTime_anim >= nowAnimState.currentTotalPlayTime_anim)
	{
		nowAnimState.currentPlayTime_anim = static_cast<float>(fmod(nowAnimState.currentPlayTime_anim, nowAnimState.currentTotalPlayTime_anim));
	}

	// 再生時間をセットする
	MV1SetAttachAnimTime(modelHandle, nowAnimState.currentAttachIndex, nowAnimState.currentPlayTime_anim);
}