#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Run.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle"></param>
<<<<<<< HEAD
Run::Run(int modelHandle, int& prevAttachIndex, NowAnimState nowAnimState) :
	PlayerStateActionBase(modelHandle, prevAttachIndex,nowAnimState)
{
	// �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
	this->nowAnimState.currentAttachIndex = MV1AttachAnim(modelHandle, animNum::Run);

	this->nowAnimState.currentPlayAnimSpeed = playAnimSpeed;
=======
Run::Run(int modelHandle) : PlayerStateActionBase(modelHandle)
{
	currentPlayAnimSpeed = playAnimSpeed;
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Run::~Run()
{
<<<<<<< HEAD
	MV1DetachAnim(modelHandle, nowAnimState.currentAttachIndex);
=======
	MV1DetachAnim(modelHandle, currentAttachIndex);
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c
}