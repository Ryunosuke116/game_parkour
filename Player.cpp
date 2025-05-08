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
	ChangeMotion(Idle, PlayAnimSpeed);
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update(const Input& input, const VECTOR& cameraDirection)
{
	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);

    Move(input, moveVec, cameraDirection);

    //�����Ă��Ȃ��Ƃ�
    if (!isMove)
    {
        ChangeMotion(Idle, PlayAnimSpeed);
    }

    if (VSize(moveVec) != 0)
    {
        targetMoveDirection = moveVec;
    }

    //�i�ރX�s�[�h����Z
    moveVec = VScale(moveVec, MoveSpeed);

    position = VAdd(position, moveVec);

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
        if (animNumber != Run)
        {
            ChangeMotion(Run, PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, upMove);
        isMove = true;
    }

    //�����͂��ꂽ�Ƃ�
    if (padInput.isDown(input))
    {
        if (animNumber != Run)
        {
            ChangeMotion(Run, PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, VScale(upMove, -1.0f));
        isMove = true;
    }

    //�����͂��ꂽ�Ƃ�
    if (padInput.isLeft(input))
    {
        if (animNumber != Run)
        {
            ChangeMotion(Run, PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, rightMove);
        isMove = true;
    }

    //�E���͂��ꂽ�Ƃ�
    if (padInput.isRight(input))
    {
        if (animNumber != Run)
        {
            ChangeMotion(Run, PlayAnimSpeed);
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
/// �v���C���[�̉�]����
/// </summary>
void Player::UpdateAngle(const VECTOR direction)
{
    // �v���C���[�̈ړ������Ƀ��f���̕������߂Â���
    float targetAngle;			// �ڕW�p�x
    float difference;			// �ڕW�p�x�ƌ��݂̊p�x�Ƃ̍�

    // �ڕW�̕����x�N�g������p�x�l���Z�o����
    targetAngle = static_cast<float>(atan2(direction.x, direction.z));

    // �ڕW�̊p�x�ƌ��݂̊p�x�Ƃ̍�������o��
    // �ŏ��͒P���Ɉ����Z
    difference = targetAngle - angle;

    // ����������炠������̍����P�W�O�x�ȏ�ɂȂ邱�Ƃ͖����̂�
    // ���̒l���P�W�O�x�ȏ�ɂȂ��Ă�����C������
    if (difference < -DX_PI_F)
    {
        difference += DX_TWO_PI_F;
    }
    else if (difference > DX_PI_F)
    {
        difference -= DX_TWO_PI_F;
    }

    // �p�x�̍����O�ɋ߂Â���
    if (difference > 0.0f)
    {
        // �����v���X�̏ꍇ�͈���
        difference -= angleSpeed;
        if (difference < 0.0f)
        {
            difference = 0.0f;
        }
    }
    else
    {
        // �����}�C�i�X�̏ꍇ�͑���
        difference += angleSpeed;
        if (difference > 0.0f)
        {
            difference = 0.0f;
        }
    }

    // ���f���̊p�x���X�V
    angle = targetAngle - difference;
    MV1SetRotationXYZ(modelHandle, VGet(0.0f, angle + DX_PI_F, 0.0f));
}

/// <summary>
/// ���[�V�����ύX
/// </summary>
/// <param name="motionNum"></param>
void Player::ChangeMotion(const int& motionNum, const float playAnimSpeed)
{
    if (animNumber == motionNum)
    {
        return;
    }
    BaseChara::ChangeMotion(motionNum, playAnimSpeed);
    animNumber = motionNum;
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