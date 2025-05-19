#include <iostream>
#include <memory>
#include "DxLib.h"
#include "PlayerStateActionBase.h"
#include "Input.h"
#include "Quick_Roll.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldAnimState"></param>
/// <param name="nowAnimState"></param>
Quick_Roll::Quick_Roll(int& modelHandle,
	OldAnimState& oldAnimState, NowAnimState& nowAnimState) :
	PlayerStateActionBase(modelHandle, oldAnimState, nowAnimState)
{
	// �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
	this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::jump_Over);

	this->nowAnimState.PlayTime_anim = 0.0f;
	this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Quick_Roll::~Quick_Roll()
{

}

/// <summary>
/// �A�j���[�V�����X�V
/// </summary>
/// <param name="playerData"></param>
/// <returns></returns>
bool Quick_Roll::MotionUpdate(PlayerData& playerData)
{
    float totalTime_anim;
    float flag = false;

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
        // �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.AttachIndex);

        //�Đ����ԍX�V

        //�󒆂ɂ���Ƃ�40f�ŃA�j���[�V�������~�߂�
        if (!playerData.isGround)
        {
          /*  if (nowAnimState.PlayTime_anim <= 33.0f)
            {
                nowAnimState.PlayTime_anim += nowAnimState.PlayAnimSpeed;
       
            }*/
        }
 
            nowAnimState.PlayTime_anim += nowAnimState.PlayAnimSpeed;

        if (nowAnimState.PlayTime_anim >= 40.0f)
        {
            if (!playerData.isGround)
            {
               // playerData.isRoll = false;
                flag = true;
            }
        }

        //���Đ����Ԃ𒴂����烊�Z�b�g
        if (nowAnimState.PlayTime_anim >= totalTime_anim)
        {
            playerData.isRoll = false;

           // nowAnimState.PlayTime_anim = static_cast<float>(fmod(nowAnimState.PlayTime_anim, totalTime_anim));
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