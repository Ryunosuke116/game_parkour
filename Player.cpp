#include"playerState.h"
#include "Include.h"

/// <summary>
/// /�C���X�g���N�^
/// </summary>
Player::Player() :
    centerPosition(VGet(0.0f, 0.0f, 0.0f)),
    footPosition(VGet(0.0f, 0.0f, 0.0f)),
    moveVec(VGet(0.0f, 0.0f, 0.0f))
{
    modelHandle = MV1LoadModel("material/mv1/human/human_0519.mv1");
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
    position = VGet(2145.0f, 0.0f, 917.0f);
    position = VGet(1070.0f, 0.0f, 450.0f);
    position = VGet(0.0f, 10.0f, 0.0f);

    MV1SetRotationXYZ(modelHandle, VGet(0, 0, 0));
   // ChangeMotion(animNum::idle, PlayAnimSpeed);
    currentJumpSpeed = 0.0f;
    isPush = false;
    isChageState = false;
    playerData.isJump = false;
    playerData.isJump_second = false;
    playerData.isMove = false;
    playerData.isRoll = false;
    playerData.isGround = true;
    playerData.isSprint = false;
    playerData.isStopRun = false;
    playerData.isJumpAll = false;
    playerData.isRoll_PlayAnim = false;
    playerData.isJump_PlayAnim = false;
   
    oldAnimState.AttachIndex = -1;
    oldAnimState.PlayAnimSpeed = 0.0f;
    oldAnimState.PlayTime_anim = 0.0f;
    oldAnimState.TotalPlayTime_anim = 0.0f;
    nowAnimState.AttachIndex = -1;
    nowAnimState.PlayAnimSpeed = 0.0f;
    nowAnimState.PlayTime_anim = 0.0f;
    nowAnimState.TotalPlayTime_anim = 0.0f;
    nowMoveSpeed = 0.0f;
    
    animNumber_Now = animNum::idle;
    nowState = std::make_shared<Idle>(modelHandle,oldAnimState, nowAnimState, playerData);
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update(const VECTOR& cameraDirection)
{
    moveVec = VGet(0.0f, 0.0f, 0.0f);

    //�ڒn���Ă���ꍇ���Z�b�g����
    if (playerData.isGround)
    {
        //position.y = 0.0f;
      //  playerData.isGround = true;
        playerData.isJump = false;
        playerData.isJump_second = false;
        playerData.isJumpAll = false;
        playerData.isRoll_PlayAnim = false;
        currentJumpSpeed = 0.0f;
    }

    this->input->Update();

    Move(moveVec, cameraDirection);
    RollMove();
    JumpMove();

    ChangeState();

    isChageState = nowState->MotionUpdate(playerData);

    //�ړ������x�N�g����0�łȂ��ꍇ�R�s�[
    if (VSize(moveVec) != 0)
    {
        targetMoveDirection = moveVec;
    }

    RollCalclation(moveVec);


    //�i�ރX�s�[�h����Z
    //���[���A�N�V�������͂���ɉ��������x
    if (playerData.isRoll)
    {
        moveVec = VScale(moveVec, rollMoveSpeed);
    }
    else
    {
        MoveCalc(moveVec);
    }

    //�W�����v�v�Z
    JumpCalclation(nowState->GetNowAnimState().PlayTime_anim,moveVec);

    //�d�͌v�Z
    GravityCalclation();

    //position = VAdd(position, moveVec);

    //// �v���C���[�̃��f���̍��W���X�V����
    //MV1SetPosition(modelHandle, position);

    UpdateAngle(targetMoveDirection);

    if (CheckHitKey(KEY_INPUT_1))
    {
        nowFrameNumber++;
    }
    else if (CheckHitKey(KEY_INPUT_2))
    {
        nowFrameNumber--;
    }

    //2����
    //0�^��
    centerPosition = MV1GetFramePosition(modelHandle, 2);
    footPosition = MV1GetFramePosition(modelHandle, 0);

    topPosition = position;
    bottomPosition = position;
    topPosition.y = topPosition.y + addTopPos;
    bottomPosition.y = bottomPosition.y + addBottomPos;

    SettingRay();
}

/// <summary>
/// �`��
/// </summary>
void Player::Draw()
{
	MV1DrawModel(modelHandle);
    //DrawSphere3D(bottomPosition, 3.5f, 30, GetColor(0, 0, 0),
    //    GetColor(255, 0, 0), FALSE);
    DrawCapsule3D(topPosition, bottomPosition, radius, 30, GetColor(0, 0, 0),
        GetColor(255, 0, 0), FALSE);

    printfDx("playerPosition.x %f\nplayerPosition.y %f\nplayerPosition.z %f\n",
        position.x, position.y, position.z);
    printfDx("frame���ݐ�%d\n", nowFrameNumber);
    printfDx("nowMoveSpeed %f\n", nowMoveSpeed);
    printfDx("isMove %d\n", playerData.isMove);
    printfDx("isJump %d\n", playerData.isJump);
    printfDx("isJump_second %d\n", playerData.isJump_second);
    printfDx("isGround %d\n", playerData.isGround);
    printfDx("isRoll %d\n", playerData.isRoll);
    printfDx("isSprint %d\n", playerData.isSprint);
    printfDx("isStopRun %d\n", playerData.isStopRun);
    printfDx("isJumpAll %d\n", playerData.isJumpAll);
    printfDx("isChageState %d\n", isChageState);
    printfDx("animNumber_Now %d\n", animNumber_Now);
    printfDx("currentJumpSpeed %f\n", currentJumpSpeed);
    nowState->Draw();

    //��
   // DrawLine3D(centerPosition, footPosition, GetColor(255, 0, 0));
    DrawLine3D(topPosition, linePos_end, GetColor(255, 0, 0));
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
        playerData.isStopRun = true;
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
        playerData.isStopRun = true;
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
        playerData.isStopRun = true;
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
        playerData.isStopRun = true;
    }

    //0�łȂ���ΐ��K��
    if (VSize(moveVec) != 0)
    {
        moveVec = VNorm(moveVec);
    }
}

/// <summary>
/// �ړ��X�s�[�h�v�Z
/// </summary>
void Player::MoveCalc(VECTOR& moveVec)
{
    if (playerData.isMove)
    {
        //���X�ɃX�s�[�h���グ��
        nowMoveSpeed += 0.05f;

        if (nowMoveSpeed >= MaxMoveSpeed)
        {
            nowMoveSpeed = MaxMoveSpeed;
        }
    }
    else
    {
        //�ڒn���Ă���Ƃ��Ɏ~�܂����炷���~�܂�
        if (playerData.isGround)
        {
            nowMoveSpeed = 0.0f;
        }
        else
        {
            nowMoveSpeed -= 0.02f;

            if (nowMoveSpeed <= 0.0f)
            {
                nowMoveSpeed = 0.0f;
            }
        }
    }

    moveVec = VScale(targetMoveDirection, nowMoveSpeed);
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
            if (animNumber_Now == animNum::jump || animNumber_Now == animNum::run_Jump)
            {
                playerData.isJumpAll = true;
            }
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
void Player::JumpCalclation(float playTime_anim,VECTOR& moveVec)
{
    //currentPlayTime_anim > 5.0f && isJump
    if (playerData.isJump && animNumber_Now == animNum::jump)
    {
        moveVec.y += currentJumpSpeed;
    }
    //�����W�����v�̂Ƃ��͑����ɉ��Z
    else if (animNumber_Now == animNum::run_Jump ||
        animNumber_Now == animNum::falling_Idle)
    {
        moveVec.y += currentJumpSpeed;
    }
    else if (!playerData.isGround)
    {
        moveVec.y += currentJumpSpeed;
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

   /* else
    {
        playerData.isGround = false;
    }*/

    if (!playerData.isGround)
    {
        currentJumpSpeed += gravity;
    }

}

/// <summary>
/// ���[��
/// </summary>
void Player::RollMove()
{
    if (padInput.isRoll(*input) && !playerData.isRoll_PlayAnim)
    {
        playerData.isRoll = true;
    }

}

/// <summary>
/// ���[���v�Z
/// </summary>
void Player::RollCalclation(VECTOR& moveVec)
{
    if (playerData.isRoll)
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
    if (!playerData.isMove && !playerData.isJump && !playerData.isRoll && !playerData.isStopRun &&
        animNumber_Now != animNum::idle)
    {
        //nowState���̃A�j���[�V��������ۑ�
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //nowState�̒��g���폜
        nowState = nullptr;
        animNumber_Now = animNum::idle;

        //nowState���X�V
        nowState = std::make_shared<Idle>(modelHandle,oldAnimState, nowAnimState,playerData);
    
    }

    //����
    if (playerData.isMove && !playerData.isRoll && playerData.isSprint &&
        playerData.isGround && animNumber_Now != animNum::run)
    {
        //nowState���̃A�j���[�V��������ۑ�
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //nowState�̒��g���폜
        nowState = nullptr;
        animNumber_Now = animNum::run;

        //nowState���X�V
        nowState = std::make_shared<Run>(modelHandle, oldAnimState,nowAnimState);
    }
    
    //�W�����v
    if (!playerData.isMove && (playerData.isJump || playerData.isJump_second) && !playerData.isJumpAll &&
        animNumber_Now != animNum::jump && padInput.isJump(*input))
    {
         SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;
        animNumber_Now = animNum::jump;

        //nowState���X�V
        nowState = std::make_shared<Jump>(modelHandle, oldAnimState, nowAnimState);

        playerData.isRoll = false;

        if (playerData.isJump && playerData.isJump_second)
        {
            playerData.isJumpAll = true;
        }
    }

    //�����W�����v
    if (playerData.isMove && (playerData.isJump || playerData.isJump_second) && !playerData.isJumpAll &&
        animNumber_Now != animNum::run_Jump && padInput.isJump(*input))
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        // prevAttachIndex = nowState->GetPrevAttachIndex();
        nowState = nullptr;
        animNumber_Now = animNum::run_Jump;
        nowState = std::make_shared<Run_Jump>(modelHandle, oldAnimState, nowAnimState);

        playerData.isRoll = false;

        if (playerData.isJump && playerData.isJump_second)
        {
            playerData.isJumpAll = true;
        }
    }
    
    //������
    if (isChageState && playerData.isJump && !playerData.isRoll
        && animNumber_Now != animNum::falling_Idle)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;
        animNumber_Now = animNum::falling_Idle;
        nowState = std::make_shared<Falling_Idle>(modelHandle, oldAnimState, nowAnimState);

    }

    //�]����
    if (playerData.isRoll && animNumber_Now != animNum::quick_Roll && !playerData.isRoll_PlayAnim &&
        padInput.isRoll(*input))
    {
        playerData.isJump_second = false;
        playerData.isJumpAll = false;

        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;
        animNumber_Now = animNum::quick_Roll;
        nowState = std::make_shared<Quick_Roll>(modelHandle, oldAnimState, nowAnimState);

        if (!playerData.isGround)
        {
            playerData.isRoll_PlayAnim = true;
        }
    }

    //����o��
    if (!playerData.isSprint && playerData.isMove && !playerData.isRoll && !playerData.isJump &&
        animNumber_Now != animNum::idle_To_Sprint)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;
        animNumber_Now = animNum::idle_To_Sprint;
        nowState = std::make_shared<Idle_To_Sprint>(modelHandle, oldAnimState, nowAnimState);
    }

    //����I���
    if (playerData.isStopRun && !playerData.isMove && playerData.isGround && !playerData.isRoll && !playerData.isJump &&
        animNumber_Now != animNum::run_To_Stop)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;
        animNumber_Now = animNum::run_To_Stop;
        nowState = std::make_shared<Run_To_Stop>(modelHandle, oldAnimState, nowAnimState);
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

void Player::SettingRay()
{
    linePos_end = VAdd(position, VScale(targetMoveDirection, 10.0f));
    linePos_end.y = topPosition.y - 1.0f;
  
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

void Player::Update() {};