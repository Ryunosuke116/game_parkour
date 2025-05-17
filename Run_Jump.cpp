#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Input.h"
#include "Run_Jump.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldAnimState"></param>
/// <param name="nowAnimState"></param>
Run_Jump::Run_Jump(int& modelHandle,
	OldAnimState& oldAnimState, NowAnimState& nowAnimState) :
	PlayerStateActionBase(modelHandle, oldAnimState, nowAnimState)
{
	input = std::make_shared<Input>();

	//MV1DetachAnim(this->modelHandle, this->nowAnimState.AttachIndex);

	// �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
	this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::run_Jump);

	this->nowAnimState.PlayTime_anim = 0.0f;
	this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
	animBlendRate = 1.0f;
	isPush = false;

	//�A�j���[�V�����̃��f���ɑ΂��锽�f�����Z�b�g
	MV1SetAttachAnimBlendRate(modelHandle, this->nowAnimState.AttachIndex, animBlendRate);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Run_Jump::~Run_Jump()
{

}


bool Run_Jump::MotionUpdate(PlayerData& playerData)
{
	bool flag = false;

	if (!isPush)
	{
 		if (playerData.isJump_second)
		{
			nowAnimState.PlayTime_anim = 0.0f;
			isPush = true;
		}
	}
	
	if (nowAnimState.PlayTime_anim >= 22.0f)
	{
		flag = true;
	}

	PlayerStateActionBase::MotionUpdate(playerData);

	return flag;
}