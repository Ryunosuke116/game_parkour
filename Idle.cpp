#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Idle.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle"></param>
Idle::Idle(int& modelHandle,
    OldAnimState& oldAnimState, NowAnimState& nowAnimState, PlayerData& playerData) :
    PlayerStateActionBase(modelHandle, oldAnimState,nowAnimState)
{
    // �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::idle);

    this->nowAnimState.PlayTime_anim = 0.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
    playerData.isSprint = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Idle::~Idle()
{
  //  MV1DetachAnim(modelHandle, this->nowAnimState.AttachIndex);
}