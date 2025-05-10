#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Run.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Run::Run(int modelHandle, int& prevAttachIndex, NowAnimState nowAnimState) :
	PlayerStateActionBase(modelHandle, prevAttachIndex,nowAnimState)
{
	// ３Ｄモデルの０番目のアニメーションをアタッチする
	this->nowAnimState.currentAttachIndex = MV1AttachAnim(modelHandle, animNum::Run);

	this->nowAnimState.currentPlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
Run::~Run()
{
	MV1DetachAnim(modelHandle, nowAnimState.currentAttachIndex);
}