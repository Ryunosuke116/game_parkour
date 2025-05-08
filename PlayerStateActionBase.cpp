#include <iostream>
#include <vector>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle"></param>
PlayerStateActionBase::PlayerStateActionBase(int modelHandle)
{
	this->modelHandle = modelHandle;

    if (prevAttachIndex != -1)
    {
        MV1DetachAnim(modelHandle, prevAttachIndex);
        prevAttachIndex = -1;
    }

    //���܂܂ŏ���prev�ɕۑ�
    prevAttachIndex = currentAttachIndex;
    prevPlayTime_anim = currentPlayTime_anim;
    prevPlayAnimSpeed = currentPlayAnimSpeed;

    // �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
    currentAttachIndex = MV1AttachAnim(modelHandle, animNum::Idle);

    // �Đ����Ԃ̏�����
    currentPlayTime_anim = 0.0f;

    // �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
    currentTotalPlayTime_anim = MV1GetAttachAnimTotalTime(modelHandle, currentAttachIndex);

    // �u�����h����Prev���L���ł͂Ȃ��ꍇ�͂P�D�O��( ���݃��[�V�������P�O�O���̏�� )�ɂ���
    animBlendRate = prevAttachIndex == -1 ? 1.0f : 0.0f;
}

void PlayerStateActionBase::MotionUpdate()
{
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

    if (currentAttachIndex != -1)
    {
        // �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, currentAttachIndex);

        //�Đ����ԍX�V
        currentPlayTime_anim += currentPlayAnimSpeed;

        //���Đ����Ԃ𒴂����烊�Z�b�g
        if (currentPlayTime_anim >= totalTime_anim)
        {
            currentPlayTime_anim = static_cast<float>(fmod(currentPlayTime_anim, totalTime_anim));
        }

        // �Đ����Ԃ��Z�b�g����
        MV1SetAttachAnimTime(modelHandle, currentAttachIndex, currentPlayTime_anim);

        //�A�j���[�V�����̃��f���ɑ΂��锽�f�����Z�b�g
        MV1SetAttachAnimBlendRate(modelHandle, currentAttachIndex, animBlendRate);
    }


    //�Đ����Ă���A�j���[�V�����Q�̏���
    if (prevAttachIndex != -1)
    {
        // �A�j���[�V�����̑����Ԃ��擾
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, prevAttachIndex);

        // �Đ����Ԃ�i�߂�
        prevPlayTime_anim += prevPlayAnimSpeed;

        // �Đ����Ԃ������Ԃɓ��B���Ă�����Đ����Ԃ����[�v������
        if (prevPlayTime_anim > totalTime_anim)
        {
            prevPlayTime_anim = static_cast<float>(fmod(prevPlayTime_anim, totalTime_anim));
        }

        // �ύX�����Đ����Ԃ����f���ɔ��f������
        MV1SetAttachAnimTime(modelHandle, prevAttachIndex, prevPlayTime_anim);

        // �A�j���[�V�����Q�̃��f���ɑ΂��锽�f�����Z�b�g
        MV1SetAttachAnimBlendRate(modelHandle, prevAttachIndex, 1.0f - animBlendRate);
    }
}

