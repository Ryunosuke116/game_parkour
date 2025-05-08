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
	ChangeMotion(Idle, PlayAnimSpeed);
}

/// <summary>
/// 更新
/// </summary>
void Player::Update(const Input& input, const VECTOR& cameraDirection)
{
	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);

    Move(input, moveVec, cameraDirection);

    //動いていないとき
    if (!isMove)
    {
        ChangeMotion(Idle, PlayAnimSpeed);
    }

    if (VSize(moveVec) != 0)
    {
        targetMoveDirection = moveVec;
    }

    //進むスピードを乗算
    moveVec = VScale(moveVec, MoveSpeed);

    position = VAdd(position, moveVec);

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
        if (animNumber != Run)
        {
            ChangeMotion(Run, PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, upMove);
        isMove = true;
    }

    //下入力されたとき
    if (padInput.isDown(input))
    {
        if (animNumber != Run)
        {
            ChangeMotion(Run, PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, VScale(upMove, -1.0f));
        isMove = true;
    }

    //左入力されたとき
    if (padInput.isLeft(input))
    {
        if (animNumber != Run)
        {
            ChangeMotion(Run, PlayAnimSpeed);
        }
        moveVec = VAdd(moveVec, rightMove);
        isMove = true;
    }

    //右入力されたとき
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
/// プレイヤーの回転制御
/// </summary>
void Player::UpdateAngle(const VECTOR direction)
{
    // プレイヤーの移動方向にモデルの方向を近づける
    float targetAngle;			// 目標角度
    float difference;			// 目標角度と現在の角度との差

    // 目標の方向ベクトルから角度値を算出する
    targetAngle = static_cast<float>(atan2(direction.x, direction.z));

    // 目標の角度と現在の角度との差を割り出す
    // 最初は単純に引き算
    difference = targetAngle - angle;

    // ある方向からある方向の差が１８０度以上になることは無いので
    // 差の値が１８０度以上になっていたら修正する
    if (difference < -DX_PI_F)
    {
        difference += DX_TWO_PI_F;
    }
    else if (difference > DX_PI_F)
    {
        difference -= DX_TWO_PI_F;
    }

    // 角度の差が０に近づける
    if (difference > 0.0f)
    {
        // 差がプラスの場合は引く
        difference -= angleSpeed;
        if (difference < 0.0f)
        {
            difference = 0.0f;
        }
    }
    else
    {
        // 差がマイナスの場合は足す
        difference += angleSpeed;
        if (difference > 0.0f)
        {
            difference = 0.0f;
        }
    }

    // モデルの角度を更新
    angle = targetAngle - difference;
    MV1SetRotationXYZ(modelHandle, VGet(0.0f, angle + DX_PI_F, 0.0f));
}

/// <summary>
/// モーション変更
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