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
    ChangeMotion(static_cast<int>(animNum::Idle), PlayAnimSpeed);
    currentJumpSpeed = 0.0f;
    isMove = false;
    isJump = false;
    isJump_second = false;
    isPush = false;
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update(const Input& input, const VECTOR& cameraDirection)
{
	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);

    Move(input, moveVec, cameraDirection);
    Jump(input);

    //�����Ă��Ȃ��Ƃ�
    if (!isMove && !isJump)
    {
        ChangeMotion(static_cast<int>(animNum::Idle), PlayAnimSpeed);
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

	MotionUpdate();

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
        if (animNumber_Now != static_cast<int>(animNum::Run) && !isJump)
        {
            ChangeMotion(static_cast<int>(animNum::Run), PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, upMove);
        isMove = true;
    }

    //�����͂��ꂽ�Ƃ�
    if (padInput.isDown(input))
    {
        if (animNumber_Now != static_cast<int>(animNum::Run) && !isJump)
        {
            ChangeMotion(static_cast<int>(animNum::Run), PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, VScale(upMove, -1.0f));
        isMove = true;
    }

    //�����͂��ꂽ�Ƃ�
    if (padInput.isLeft(input))
    {
        if (animNumber_Now != static_cast<int>(animNum::Run) && !isJump)
        {
            ChangeMotion(static_cast<int>(animNum::Run), PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, rightMove);
        isMove = true;
    }

    //�E���͂��ꂽ�Ƃ�
    if (padInput.isRight(input))
    {
        if (animNumber_Now != static_cast<int>(animNum::Run) && !isJump)
        {
            ChangeMotion(static_cast<int>(animNum::Run), PlayAnimSpeed);
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
void Player::Jump(const Input& input)
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
                ChangeMotion(static_cast<int>(animNum::Run_Jump), PlayAnimSpeed);
            }
            else
            {
                ChangeMotion(static_cast<int>(animNum::Jump), PlayAnimSpeed);
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
                ChangeMotion(static_cast<int>(animNum::Run_Jump), PlayAnimSpeed);
            }
            else
            {
                ChangeMotion(static_cast<int>(animNum::Jump), PlayAnimSpeed);
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
   /* else if (animNumber_Now == static_cast<int>(animNum::Run_Jump))
    {
        position.y += currentJumpSpeed;
    }*/

    //�d�͂����Z����
    if (isJump && currentPlayTime_anim > 5.0f && animNumber_Now == static_cast<int>(animNum::Jump))
    {
        currentJumpSpeed += -gravity;
    }
    else if (isJump && animNumber_Now == static_cast<int>(animNum::Run_Jump))
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
    if (motionNum == static_cast<int>(animNum::Jump))
    {
        if (keepPlayTime_anim >= totalTime_anim)
        {
             ChangeMotion(static_cast<int>(animNum::Falling_Idle), PlayAnimSpeed);
        }
    }
}

/// <summary>
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