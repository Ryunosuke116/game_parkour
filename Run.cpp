#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Run.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle"></param>
Run::Run(int modelHandle) : PlayerStateActionBase(modelHandle)
{
	currentPlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Run::~Run()
{
	MV1DetachAnim(modelHandle, currentAttachIndex);
}