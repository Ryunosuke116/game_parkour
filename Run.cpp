#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Run.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle"></param>
Run::Run(int& modelHandle, 
	OldAnimState& oldAnimState, NowAnimState& nowAnimState) :
	PlayerStateActionBase(modelHandle,  oldAnimState,nowAnimState)
{
	// �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
	this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::Run);

	this->nowAnimState.PlayTime_anim = 0.0f;
	this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Run::~Run()
{
//	MV1DetachAnim(modelHandle, nowAnimState.AttachIndex);
}