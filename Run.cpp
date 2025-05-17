#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Run.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Run::Run(int& modelHandle, 
	OldAnimState& oldAnimState, NowAnimState& nowAnimState) :
	PlayerStateActionBase(modelHandle,  oldAnimState,nowAnimState)
{
	// ３Ｄモデルの０番目のアニメーションをアタッチする
	this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::Run);

	this->nowAnimState.PlayTime_anim = 0.0f;
	this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
Run::~Run()
{
//	MV1DetachAnim(modelHandle, nowAnimState.AttachIndex);
}