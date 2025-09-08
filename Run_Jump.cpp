#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Input.h"
#include "runJump.h"
#include "AnimTime.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldAnimState"></param>
/// <param name="nowAnimState"></param>
runJump::runJump(int& modelHandle,
	AnimState& oldAnimState, AnimState& nowAnimState) :
	PlayerStateActionBase(modelHandle, oldAnimState, nowAnimState)
{
	input = std::make_shared<Input>();

	//MV1DetachAnim(this->modelHandle, this->nowAnimState.AttachIndex);

	// ３Ｄモデルの０番目のアニメーションをアタッチする
	this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::runJump);

	this->nowAnimState.PlayTime_anim = 0.0f;
	this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
	animBlendRate = 1.0f;
	isPush = false;

	//アニメーションのモデルに対する反映率をセット
	MV1SetAttachAnimBlendRate(modelHandle, this->nowAnimState.AttachIndex, animBlendRate);
}

/// <summary>
/// デストラクタ
/// </summary>
runJump::~runJump()
{

}


bool runJump::MotionUpdate(PlayerData& playerData)
{
	bool flag = false;

	if (!isPush)
	{
		//二段ジャンプしたらプレイタイムリセット
 		if (playerData.isJumpSecond)
		{
			nowAnimState.PlayTime_anim = 0.0f;
			isPush = true;
		}
	}
	
	if (nowAnimState.PlayTime_anim >= 22.0f)
	{
		flag = true;
		playerData.isJump_run_playAnim = false;
	}

	PlayerStateActionBase::MotionUpdate(playerData);

	return flag;
}