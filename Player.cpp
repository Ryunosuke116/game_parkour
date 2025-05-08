#include "Include.h"

/// <summary>
/// /インストラクタ
/// </summary>
Player::Player()
{
	modelHandle = MV1LoadModel("material/mv1/human/human_0501.mv1");
	MV1SetScale(modelHandle, VGet(modelScale, modelScale, modelScale));
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{

}

/// <summary>
/// 初期化
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
/// 更新
/// </summary>
void Player::Update(const Input& input, const VECTOR& cameraDirection)
{
	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);

    Move(input, moveVec, cameraDirection);
    Jump(input);

    //動いていないとき
    if (!isMove && !isJump)
    {
        ChangeMotion(static_cast<int>(animNum::Idle), PlayAnimSpeed);
    }

    if (VSize(moveVec) != 0)
    {
        targetMoveDirection = moveVec;
    }

    //進むスピードを乗算
    moveVec = VScale(moveVec, MoveSpeed);

    position = VAdd(position, moveVec);

    JumpCalclation();

    // プレイヤーのモデルの座標を更新する
    MV1SetPosition(modelHandle, position);

	MotionUpdate();

    UpdateAngle(targetMoveDirection);
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	MV1DrawModel(modelHandle);
    printfDx("playerPosition.x %f\nplayerPosition.y %f\nplayerPosition.x %f\n",
        position.x, position.y, position.z);
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="input"></param>
/// <param name="moveVec"></param>
void Player::Move(const Input& input, VECTOR& moveVec, const VECTOR& cameraDirection)
{

    isMove = false;
    VECTOR rightMove = VCross(cameraDirection, VGet(0.0f, 1.0f, 0.0f));

    //正規化
    rightMove = VNorm(rightMove);
    VECTOR upMove = VNorm(cameraDirection);

    upMove.y = 0.0f;
    rightMove.y = 0.0f;

    //上入力されたとき
    if (padInput.isUp(input))
    {
        if (animNumber_Now != static_cast<int>(animNum::Run) && !isJump)
        {
            ChangeMotion(static_cast<int>(animNum::Run), PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, upMove);
        isMove = true;
    }

    //下入力されたとき
    if (padInput.isDown(input))
    {
        if (animNumber_Now != static_cast<int>(animNum::Run) && !isJump)
        {
            ChangeMotion(static_cast<int>(animNum::Run), PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, VScale(upMove, -1.0f));
        isMove = true;
    }

    //左入力されたとき
    if (padInput.isLeft(input))
    {
        if (animNumber_Now != static_cast<int>(animNum::Run) && !isJump)
        {
            ChangeMotion(static_cast<int>(animNum::Run), PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, rightMove);
        isMove = true;
    }

    //右入力されたとき
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
/// ジャンプ
/// </summary>
void Player::Jump(const Input& input)
{
    if (padInput.isJump(input))
    {
        //一回目のジャンプ
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
        //二段ジャンプ
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
/// ジャンプ処理
/// </summary>
void Player::JumpCalclation()
{
    //ジャンプの時は少し遅らせて加算する
    //currentPlayTime_anim > 5.0f && isJump
    if (isJump)
    {
        position.y += currentJumpSpeed;
    }
    //ランジャンプのときは即座に加算
   /* else if (animNumber_Now == static_cast<int>(animNum::Run_Jump))
    {
        position.y += currentJumpSpeed;
    }*/

    //重力を加算する
    if (isJump && currentPlayTime_anim > 5.0f && animNumber_Now == static_cast<int>(animNum::Jump))
    {
        currentJumpSpeed += -gravity;
    }
    else if (isJump && animNumber_Now == static_cast<int>(animNum::Run_Jump))
    {
        currentJumpSpeed += -gravity;
    }

    //条件を満たしたらジャンプ終了
    if (position.y < 0.0f)
    {
        position.y = 0.0f;
        isJump = false;
        isJump_second = false;
    }


}

/// <summary>
/// モーション変更
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
/// 上入力
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
/// 下入力
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
/// 右入力
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
/// 左入力
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
/// ジャンプ入力
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