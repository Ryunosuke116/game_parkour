#include"playerState.h"
#include "Include.h"

/// <summary>
/// /�C���X�g���N�^
/// </summary>
Player::Player()
{
	modelHandle = MV1LoadModel("material/mv1/human/human_0501.mv1");
	MV1SetScale(modelHandle, VGet(modelScale, modelScale, modelScale));
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
    isMove = false;
    isJump = false;
    isJump_second = false;
    isPush = false;
<<<<<<< HEAD
   
    this->nowAnimState.currentAttachIndex = -1;
    this->nowAnimState.currentPlayAnimSpeed = 0.0f;
    this->nowAnimState.currentPlayTime_anim = 0.0f;
    this->nowAnimState.currentTotalPlayTime_anim = 0.0f;
    nowState = std::make_shared<Idle>(modelHandle, prevAttachIndex,nowAnimState);
=======
    nowState = std::make_shared<Idle>(modelHandle);
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update(const Input& input, const VECTOR& cameraDirection)
{
	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);

    Move(input, moveVec, cameraDirection);
    JumpMove(input);

    ChangeState();

    nowState->MotionUpdate();

    //�����Ă��Ȃ��Ƃ�
    if (!isMove && !isJump)
    {
        //ChangeMotion(animNum::idle, PlayAnimSpeed);
    }

    if (VSize(moveVec) != 0)
    {
        targetMoveDirection = moveVec;
    }

    //�i�ރX�s�[�h����Z
    moveVec = VScale(moveVec, MoveSpeed);

    position = VAdd(position, moveVec);

    JumpCalclation();

    // �v���C���[�̃��f���̍��W���X�V����
    MV1SetPosition(modelHandle, position);

	//MotionUpdate();

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
void Player::Move(const Input& input, VECTOR& moveVec, const VECTOR& cameraDirection)
{

    isMove = false;
    VECTOR rightMove = VCross(cameraDirection, VGet(0.0f, 1.0f, 0.0f));

    //���K��
    rightMove = VNorm(rightMove);
    VECTOR upMove = VNorm(cameraDirection);

    upMove.y = 0.0f;
    rightMove.y = 0.0f;

    //����͂��ꂽ�Ƃ�
    if (padInput.isUp(input))
    {
        if (animNumber_Now != animNum::run && !isJump)
        {
           // ChangeMotion(animNum::run, PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, upMove);
        isMove = true;
    }

    //�����͂��ꂽ�Ƃ�
    if (padInput.isDown(input))
    {
        if (animNumber_Now != animNum::run && !isJump)
        {
           // ChangeMotion(animNum::run, PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, VScale(upMove, -1.0f));
        isMove = true;
    }

    //�����͂��ꂽ�Ƃ�
    if (padInput.isLeft(input))
    {
        if (animNumber_Now != animNum::run && !isJump)
        {
           // ChangeMotion(animNum::run, PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, rightMove);
        isMove = true;
    }

    //�E���͂��ꂽ�Ƃ�
    if (padInput.isRight(input))
    {
        if (animNumber_Now != animNum::run && !isJump)
        {
            //ChangeMotion(animNum::run, PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, VScale(rightMove, -1.0f));
        isMove = true;
    }

    if (VSize(moveVec) != 0)
    {
        moveVec = VNorm(moveVec);
    }
}

/// <summary>
/// �W�����v
/// </summary>
void Player::JumpMove(const Input& input)
{
    if (padInput.isJump(input))
    {
        //���ڂ̃W�����v
        if (!isJump)
        {
            isPush = true;
            currentJumpSpeed = addJumpPower;
            isJump = true;
            if (isMove)
            {
               // ChangeMotion(animNum::run_Jump, PlayAnimSpeed);
            }
            else
            {
               // ChangeMotion(animNum::jump, PlayAnimSpeed);
            }
        }
        //��i�W�����v
        if (isJump && !isPush && !isJump_second)
        {
            isPush = true;
            isJump_second = true;
            currentJumpSpeed = addJumpPower;
            if (isMove)
            {
               // ChangeMotion(animNum::run_Jump, PlayAnimSpeed);
            }
            else
            {
                //ChangeMotion(animNum::jump, PlayAnimSpeed);
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
void Player::JumpCalclation()
{
    //�W�����v�̎��͏����x�点�ĉ��Z����
    //currentPlayTime_anim > 5.0f && isJump
    if (isJump)
    {
        position.y += currentJumpSpeed;
    }
    //�����W�����v�̂Ƃ��͑����ɉ��Z
   /* else if (animNumber_Now == (animNum::run_Jump))
    {
        position.y += currentJumpSpeed;
    }*/

    //�d�͂����Z����
    if (isJump && currentPlayTime_anim > 5.0f && animNumber_Now == animNum::jump)
    {
        currentJumpSpeed += -gravity;
    }
    else if (isJump && animNumber_Now == animNum::run_Jump)
    {
        currentJumpSpeed += -gravity;
    }

    //�����𖞂�������W�����v�I��
    if (position.y < 0.0f)
    {
        position.y = 0.0f;
        isJump = false;
        isJump_second = false;
    }


}

/// <summary>
/// ���[�V�����ύX
/// </summary>
/// <param name="motionNum"></param>
void Player::ChangeMotion(const int& motionNum, const float playAnimSpeed)
{
    if (animNumber_Now == motionNum && !isJump)
    {
        return;
    }
    BaseChara::ChangeMotion(motionNum, playAnimSpeed);
    animNumber_Now = motionNum;
}

void Player::MotionUpdate()
{
    BaseChara::MotionUpdate();
    if (motionNum == animNum::jump)
    {
        if (keepPlayTime_anim >= totalTime_anim)
        {
             ChangeMotion(animNum::falling_Idle, PlayAnimSpeed);
        }
    }
}

void Player::ChangeState()
{

<<<<<<< HEAD
    if (!isMove && !isJump && animNumber_Now != animNum::idle)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        prevAttachIndex = nowState->GetPrevAttachIndex();
        nowState = nullptr;
        animNumber_Now = animNum::idle;
        nowState = std::make_shared<Idle>(modelHandle, prevAttachIndex,nowAnimState);
    }

    if (isMove && position.y == 0.0f && animNumber_Now != animNum::run)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        prevAttachIndex = nowState->GetPrevAttachIndex();
        nowState = nullptr;
        animNumber_Now = animNum::run;
        nowState = std::make_shared<Run>(modelHandle, prevAttachIndex,nowAnimState);
    }

    if (isJump && animNumber_Now != animNum::jump)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        prevAttachIndex = nowState->GetPrevAttachIndex();
        nowState = nullptr;
        animNumber_Now = animNum::jump;
        nowState = std::make_shared<Jump>(modelHandle, prevAttachIndex,nowAnimState);
    }

    if (isJump && currentJumpSpeed < 0.0f && animNumber_Now != animNum::falling_Idle)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        prevAttachIndex = nowState->GetPrevAttachIndex();
        nowState = nullptr;
        animNumber_Now = animNum::falling_Idle;
        nowState = std::make_shared<Falling_Idle>(modelHandle, prevAttachIndex,nowAnimState);
=======
    if (!isMove && !isJump)
    {
        nowState = nullptr;
        animNumber_Now = animNum::idle;
        nowState = std::make_shared<Idle>(modelHandle);
    }

    if (isMove && position.y == 0.0f)
    {
        nowState = nullptr;
        animNumber_Now = animNum::run;
        nowState = std::make_shared<Run>(modelHandle);
    }

    if (isJump)
    {
        nowState = nullptr;
        animNumber_Now = animNum::jump;
        nowState = std::make_shared<Jump>(modelHandle);
    }

    if (isJump && currentJumpSpeed < 0.0f)
    {
        nowState = nullptr;
        animNumber_Now = animNum::falling_Idle;
        nowState = std::make_shared<Falling_Idle>(modelHandle);
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c
    }
}

/// <summary>
<<<<<<< HEAD
/// �A�j���[�V���������Z�b�g
/// </summary>
/// <param name="AnimState"></param>
void Player::SetNowAnimState(PlayerStateActionBase::NowAnimState animState)
{
    nowAnimState.currentAttachIndex = animState.currentAttachIndex;
    nowAnimState.currentPlayAnimSpeed = animState.currentPlayAnimSpeed;
    nowAnimState.currentPlayTime_anim = animState.currentPlayTime_anim;
    nowAnimState.currentTotalPlayTime_anim = animState.currentTotalPlayTime_anim;
}

/// <summary>
=======
>>>>>>> 8b302d9ef8b09144ecd1ac2a71429c183d434d1c
/// �����
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
bool Player::PadInput::isUp(const Input& input)
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
bool Player::PadInput::isDown(const Input& input)
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
bool Player::PadInput::isRight(const Input& input)
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
bool Player::PadInput::isLeft(const Input& input)
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
bool Player::PadInput::isJump(const Input& input)
{
    if (CheckHitKey(KEY_INPUT_SPACE) ||
        input.GetNowFrameNewInput() & PAD_INPUT_A)
    {
        return true;
    }
    return false;
}