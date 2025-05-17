#include"playerState.h"
#include "Include.h"

/// <summary>
/// /�C���X�g���N�^
/// </summary>
Player::Player()
{
	modelHandle = MV1LoadModel("material/mv1/human/human_0517.mv1");
	MV1SetScale(modelHandle, VGet(modelScale, modelScale, modelScale));
    input = std::make_shared<Input>();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{

}

/// <summary>
/// ������
/// </summary>
void Player::Initialize()
{
    position = VGet(0.0f, 0.0f, 0.0f);
    MV1SetRotationXYZ(modelHandle, VGet(0, 0, 0));
   // ChangeMotion(animNum::idle, PlayAnimSpeed);
    currentJumpSpeed = 0.0f;
    isPush = false;
    isChageState = false;
    playerData.isJump = false;
    playerData.isJump_second = false;
    playerData.isMove = false;
    playerData.isRool = false;
    playerData.isGround = true;
   
    oldAnimState.AttachIndex = -1;
    oldAnimState.PlayAnimSpeed = 0.0f;
    oldAnimState.PlayTime_anim = 0.0f;
    oldAnimState.TotalPlayTime_anim = 0.0f;
    nowAnimState.AttachIndex = -1;
    nowAnimState.PlayAnimSpeed = 0.0f;
    nowAnimState.PlayTime_anim = 0.0f;
    nowAnimState.TotalPlayTime_anim = 0.0f;
    
    animNumber_Now = animNum::idle;
    nowState = std::make_shared<Idle>(modelHandle,oldAnimState, nowAnimState);
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update(const VECTOR& cameraDirection)
{
	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);

    this->input->Update();

    Move(moveVec, cameraDirection);
    RollMove();
    JumpMove();

    ChangeState();

    isChageState = nowState->MotionUpdate(playerData);

    if (VSize(moveVec) != 0)
    {
        targetMoveDirection = moveVec;
    }
    
    RollCalclation(moveVec);

    //�W�����v�v�Z
    JumpCalclation(nowState->GetNowAnimState().PlayTime_anim);

    GravityCalclation();

    //�i�ރX�s�[�h����Z
    moveVec = VScale(moveVec, MoveSpeed);

    position = VAdd(position, moveVec);


    // �v���C���[�̃��f���̍��W���X�V����
    MV1SetPosition(modelHandle, position);

    UpdateAngle(targetMoveDirection);
}

/// <summary>
/// �`��
/// </summary>
void Player::Draw()
{
	MV1DrawModel(modelHandle);
    printfDx("playerPosition.x %f\nplayerPosition.y %f\nplayerPosition.x %f\n",
        position.x, position.y, position.z);
}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="input"></param>
/// <param name="moveVec"></param>
void Player::Move(VECTOR& moveVec, const VECTOR& cameraDirection)
{

    playerData.isMove = false;
    VECTOR rightMove = VCross(cameraDirection, VGet(0.0f, 1.0f, 0.0f));

    //���K��
    rightMove = VNorm(rightMove);
    VECTOR upMove = VNorm(cameraDirection);

    upMove.y = 0.0f;
    rightMove.y = 0.0f;

    //����͂��ꂽ�Ƃ�
    if (padInput.isUp(*input))
    {
        if (animNumber_Now != animNum::run && !playerData.isJump)
        {
           // ChangeMotion(animNum::run, PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, upMove);
        playerData.isMove = true;
    }

    //�����͂��ꂽ�Ƃ�
    if (padInput.isDown(*input))
    {
        if (animNumber_Now != animNum::run && !playerData.isJump)
        {
           // ChangeMotion(animNum::run, PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, VScale(upMove, -1.0f));
        playerData.isMove = true;
    }

    //�����͂��ꂽ�Ƃ�
    if (padInput.isLeft(*input))
    {
        if (animNumber_Now != animNum::run && !playerData.isJump)
        {
           // ChangeMotion(animNum::run, PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, rightMove);
        playerData.isMove = true;
    }

    //�E���͂��ꂽ�Ƃ�
    if (padInput.isRight(*input))
    {
        if (animNumber_Now != animNum::run && !playerData.isJump)
        {
            //ChangeMotion(animNum::run, PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, VScale(rightMove, -1.0f));
        playerData.isMove = true;
    }

    //0�łȂ���ΐ��K��
    if (VSize(moveVec) != 0)
    {
        moveVec = VNorm(moveVec);
    }
}

/// <summary>
/// �W�����v
/// </summary>
void Player::JumpMove()
{
    if (padInput.isJump(*input))
    {
        //���ڂ̃W�����v
        if (!playerData.isJump)
        {
            isPush = true;
            currentJumpSpeed = addJumpPower;
            playerData.isJump = true;
        }
        //��i�W�����v
        if (playerData.isJump && !isPush && !playerData.isJump_second)
        {
            isPush = true;
            playerData.isJump_second = true;
            currentJumpSpeed = addJumpPower;
        }
    }
    else
    {
        isPush = false;
    }
}

/// <summary>
/// �W�����v����
/// </summary>
void Player::JumpCalclation(float playTime_anim)
{
    //�W�����v�̎��͏����x�点�ĉ��Z����
    //currentPlayTime_anim > 5.0f && isJump
    if (playTime_anim > 7.0f && playerData.isJump && animNumber_Now == animNum::jump)
    {
        position.y += currentJumpSpeed;
    }
    //�����W�����v�̂Ƃ��͑����ɉ��Z
    else if (animNumber_Now == animNum::run_Jump ||
        animNumber_Now == animNum::falling_Idle)
    {
        position.y += currentJumpSpeed;
    }
    else if (!playerData.isGround)
    {
        position.y += currentJumpSpeed;
    }

}

/// <summary>
/// �d�͌v�Z
/// </summary>
void Player::GravityCalclation()
{
    //�d�͂����Z����
    //if (playerData.isJump && playTime_anim > 7.0f && animNumber_Now == animNum::jump)
    //{
    //    currentJumpSpeed += -gravity;
    //}
    //else 

    if (position.y <= 0.0f)
    {
        position.y = 0.0f;
        playerData.isJump = false;
        playerData.isJump_second = false;
        playerData.isGround = true;
    }
    else
    {
        playerData.isGround = false;
    }

    if (!playerData.isGround && !playerData.isRool)
    {
        currentJumpSpeed += gravity;
    }

}

/// <summary>
/// ���[��
/// </summary>
void Player::RollMove()
{
    if (padInput.isRoll(*input))
    {
        playerData.isRool = true;
    }

}

/// <summary>
/// ���[���v�Z
/// </summary>
void Player::RollCalclation(VECTOR& moveVec)
{
    if (playerData.isRool)
    {
        moveVec = targetMoveDirection;
    }
}

/// <summary>
/// �A�j���[�V�����ύX
/// </summary>
void Player::ChangeState()
{
    //����
    if (!playerData.isMove && !playerData.isJump && !playerData.isRool &&
        animNumber_Now != animNum::idle)
    {
        //�A�j���[�V��������ۑ�
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //nowState�̒��g���폜
        nowState = nullptr;
        animNumber_Now = animNum::idle;
        nowState = std::make_shared<Idle>(modelHandle,oldAnimState, nowAnimState);
       // nowState->ResetNowAnimState();
    
    }

    //����
    if (playerData.isMove && !playerData.isRool && 
        position.y == 0.0f && animNumber_Now != animNum::run)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //prevAttachIndex = nowState->GetPrevAttachIndex();
        nowState = nullptr;
        animNumber_Now = animNum::run;
        nowState = std::make_shared<Run>(modelHandle, oldAnimState,nowAnimState);
       // nowState->ResetNowAnimState();
    }
    
    //�W�����v
    if (!playerData.isMove && playerData.isJump && animNumber_Now != animNum::jump && padInput.isJump(*input))
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

       // prevAttachIndex = nowState->GetPrevAttachIndex();
        nowState = nullptr;
        animNumber_Now = animNum::jump;
        nowState = std::make_shared<Jump>(modelHandle, oldAnimState, nowAnimState);
    }

    //�����W�����v
    if (playerData.isJump && playerData.isMove && animNumber_Now != animNum::run_Jump && padInput.isJump(*input))
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        // prevAttachIndex = nowState->GetPrevAttachIndex();
        nowState = nullptr;
        animNumber_Now = animNum::run_Jump;
        nowState = std::make_shared<Run_Jump>(modelHandle, oldAnimState, nowAnimState);
    }
    
    //������
    if (isChageState && playerData.isJump && animNumber_Now != animNum::falling_Idle)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;
        animNumber_Now = animNum::falling_Idle;
        nowState = std::make_shared<Falling_Idle>(modelHandle, oldAnimState, nowAnimState);
    }

    //�]����
    if (playerData.isRool && animNumber_Now != animNum::quick_Roll)
    {
        currentJumpSpeed = 0.0f;

        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;
        animNumber_Now = animNum::quick_Roll;
        nowState = std::make_shared<Quick_Roll>(modelHandle, oldAnimState, nowAnimState);
    }
}

/// <summary>
/// �A�j���[�V���������Z�b�g
/// </summary>
/// <param name="AnimState"></param>
void Player::SetOldAnimState(PlayerStateActionBase::OldAnimState animState)
{
    oldAnimState.AttachIndex = animState.AttachIndex;
    oldAnimState.PlayAnimSpeed = animState.PlayAnimSpeed;
    oldAnimState.PlayTime_anim = animState.PlayTime_anim;
    oldAnimState.TotalPlayTime_anim = animState.TotalPlayTime_anim;
}

void Player::SetNowAnimState(PlayerStateActionBase::NowAnimState animState)
{
    nowAnimState.AttachIndex = animState.AttachIndex;
    nowAnimState.PlayAnimSpeed = animState.PlayAnimSpeed;
    nowAnimState.PlayTime_anim = animState.PlayTime_anim;
    nowAnimState.TotalPlayTime_anim = animState.TotalPlayTime_anim;
}

/// <summary>
/// �����
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
bool Player::PadInput::isUp(Input& input)
{
    
    if (input.GetNowFrameInput() & PAD_INPUT_UP ||
        CheckHitKey(KEY_INPUT_UP))
    {
        return true;
    }

    return false;
}

/// <summary>
/// ������
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
bool Player::PadInput::isDown(Input& input)
{
    if (input.GetNowFrameInput() & PAD_INPUT_DOWN ||
        CheckHitKey(KEY_INPUT_DOWN))
    {
        return true;
    }
    return false;
}

/// <summary>
/// �E����
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
bool Player::PadInput::isRight(Input& input)
{
    if (input.GetNowFrameInput() & PAD_INPUT_RIGHT ||
        CheckHitKey(KEY_INPUT_RIGHT))
    {
        return true;
    }
    return false;
}

/// <summary>
/// ������
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
bool Player::PadInput::isLeft(Input& input)
{
    if (input.GetNowFrameInput() & PAD_INPUT_LEFT ||
        CheckHitKey(KEY_INPUT_LEFT))
    {
        return true;
    }
    return false;
}

/// <summary>
/// �W�����v����
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
bool Player::PadInput::isJump(Input& input)
{
    if (CheckHitKey(KEY_INPUT_SPACE) ||
        input.GetNowFrameNewInput() & PAD_INPUT_A)
    {
        return true;
    }
    return false;
}

/// <summary>
/// ���[������
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
bool Player::PadInput::isRoll(Input& input)
{
    if (CheckHitKey(KEY_INPUT_F) ||
        input.GetNowFrameNewInput() & PAD_INPUT_B)
    {
        return true;
    }
    return false;
}