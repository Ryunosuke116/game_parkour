#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Input.h"
#include "Jump.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle"></param>
Jump::Jump(int& modelHandle,
    OldAnimState& oldAnimState, NowAnimState& nowAnimState) :
    PlayerStateActionBase(modelHandle, oldAnimState,nowAnimState)
{
    input = std::make_shared<Input>();

    // �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::jump);

    this->nowAnimState.PlayTime_anim = 5.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
    this->nowAnimState.TotalPlayTime_anim = MV1GetAttachAnimTotalTime(modelHandle, this->nowAnimState.AttachIndex);
    isPush = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Jump::~Jump()
{
  //  MV1DetachAnim(modelHandle, this->nowAnimState.AttachIndex);
}

/// <summary>
/// �A�j���[�V�����X�V 
/// </summary>
/// <returns></returns>
bool Jump::MotionUpdate(PlayerData& playerData)
{
    input->Update();

    bool flag = false;

    float totalTime_anim;

    // �u�����h�����P�ȉ��̏ꍇ�͂P�ɋ߂Â���
    if (animBlendRate < 1.0f)
    {
        animBlendRate += AnimBlendSpeed;
        if (animBlendRate > 1.0f)
        {
            animBlendRate = 1.0f;
        }
    }

    if (nowAnimState.AttachIndex != -1)
    {
        //��i�W�����v������v���C�^�C�����Z�b�g
        if (!isPush)
        {
            if (playerData.isJump_second)
            {
                nowAnimState.PlayTime_anim = 5.0f;
                isPush = true;
            }
        }

        // �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.AttachIndex);

        //�Đ����ԍX�V
        nowAnimState.PlayTime_anim += nowAnimState.PlayAnimSpeed;

        //���Đ����Ԃ𒴂����烊�Z�b�g
        if (nowAnimState.PlayTime_anim >= totalTime_anim)
        {
            flag = true;
        }

        // �Đ����Ԃ��Z�b�g����
        MV1SetAttachAnimTime(modelHandle, nowAnimState.AttachIndex, nowAnimState.PlayTime_anim);

        //�A�j���[�V�����̃��f���ɑ΂��锽�f�����Z�b�g
        MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.AttachIndex, animBlendRate);
    }


    //�Đ����Ă���A�j���[�V�����Q�̏���
    if (oldAnimState.AttachIndex != -1)
    {
        // �A�j���[�V�����̑����Ԃ��擾
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, oldAnimState.AttachIndex);

        // �Đ����Ԃ�i�߂�
        oldAnimState.PlayTime_anim += oldAnimState.PlayAnimSpeed;

        // �Đ����Ԃ������Ԃɓ��B���Ă�����Đ����Ԃ����[�v������
        if (oldAnimState.PlayTime_anim > totalTime_anim)
        {
            oldAnimState.PlayTime_anim = static_cast<float>(fmod(oldAnimState.PlayTime_anim, totalTime_anim));
        }

        // �ύX�����Đ����Ԃ����f���ɔ��f������
        MV1SetAttachAnimTime(modelHandle, oldAnimState.AttachIndex, oldAnimState.PlayTime_anim);

        // �A�j���[�V�����Q�̃��f���ɑ΂��锽�f�����Z�b�g
        MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.AttachIndex, 1.0f - animBlendRate);
    }

    return flag;
}