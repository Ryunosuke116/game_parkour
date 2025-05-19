#include"playerState.h"
#include "Include.h"

/// <summary>
/// /インストラクタ
/// </summary>
Player::Player()
{
	modelHandle = MV1LoadModel("material/mv1/human/human_0519.mv1");
	MV1SetScale(modelHandle, VGet(modelScale, modelScale, modelScale));
    input = std::make_shared<Input>();
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
   
    oldAnimState.AttachIndex = -1;
    oldAnimState.PlayAnimSpeed = 0.0f;
    oldAnimState.PlayTime_anim = 0.0f;
    oldAnimState.TotalPlayTime_anim = 0.0f;
    nowAnimState.AttachIndex = -1;
    nowAnimState.PlayAnimSpeed = 0.0f;
    nowAnimState.PlayTime_anim = 0.0f;
    nowAnimState.TotalPlayTime_anim = 0.0f;
    
    animNumber_Now = animNum::idle;
    nowState = std::make_shared<Idle>(modelHandle,oldAnimState, nowAnimState, playerData);
}

/// <summary>
/// 更新
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

    //ジャンプ計算
    JumpCalclation(nowState->GetNowAnimState().PlayTime_anim);

    GravityCalclation();

    //進むスピードを乗算
    //ロールアクション中はそれに応じた速度
    if (playerData.isRoll)
    {
        moveVec = VScale(moveVec, rollMoveSpeed);
    }
    else
    {
        moveVec = VScale(moveVec, MoveSpeed);
    }

    position = VAdd(position, moveVec);


    // プレイヤーのモデルの座標を更新する
    MV1SetPosition(modelHandle, position);

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
    printfDx("isMove %d\n", playerData.isMove);
    printfDx("isJump %d\n", playerData.isJump);
    printfDx("isJump_second %d\n", playerData.isJump_second);
    printfDx("isGround %d\n", playerData.isGround);
    printfDx("isRoll %d\n", playerData.isRoll);
    printfDx("isSprint %d\n", playerData.isSprint);
    printfDx("isStopRun %d\n", playerData.isStopRun);
    printfDx("isChageState %d\n", isChageState);
    nowState->Draw();
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="input"></param>
/// <param name="moveVec"></param>
void Player::Move(VECTOR& moveVec, const VECTOR& cameraDirection)
{

    playerData.isMove = false;
    VECTOR rightMove = VCross(cameraDirection, VGet(0.0f, 1.0f, 0.0f));

    //正規化
    rightMove = VNorm(rightMove);
    VECTOR upMove = VNorm(cameraDirection);

    upMove.y = 0.0f;
    rightMove.y = 0.0f;

    //上入力されたとき
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

    //下入力されたとき
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

    //左入力されたとき
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

    //右入力されたとき
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

    //0でなければ正規化
    if (VSize(moveVec) != 0)
    {
        moveVec = VNorm(moveVec);
    }
}

/// <summary>
/// ジャンプ
/// </summary>
void Player::JumpMove()
{
    if (padInput.isJump(*input))
    {
        //一回目のジャンプ
        if (!playerData.isJump)
        {
            isPush = true;
            currentJumpSpeed = addJumpPower;
            playerData.isJump = true;
        }
        //二段ジャンプ
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
/// ジャンプ処理
/// </summary>
void Player::JumpCalclation(float playTime_anim)
{
    //currentPlayTime_anim > 5.0f && isJump
    if (playerData.isJump && animNumber_Now == animNum::jump)
    {
        position.y += currentJumpSpeed;
    }
    //ランジャンプのときは即座に加算
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
/// 重力計算
/// </summary>
void Player::GravityCalclation()
{
    //重力を加算する
    //if (playerData.isJump && playTime_anim > 7.0f && animNumber_Now == animNum::jump)
    //{
    //    currentJumpSpeed += -gravity;
    //}
    //else 



    if (position.y <= 0.0f)
    {
        position.y = 0.0f;
        playerData.isGround = true;
        playerData.isJump = false;
        playerData.isJump_second = false;
    }
    else
    {
        playerData.isGround = false;
    }

    if (!playerData.isGround)
    {
        currentJumpSpeed += gravity;
    }

}

/// <summary>
/// ロール
/// </summary>
void Player::RollMove()
{
    if (padInput.isRoll(*input))
    {
        playerData.isRoll = true;
    }

}

/// <summary>
/// ロール計算
/// </summary>
void Player::RollCalclation(VECTOR& moveVec)
{
    if (playerData.isRoll)
    {
        moveVec = targetMoveDirection;
    }
}

/// <summary>
/// アニメーション変更
/// </summary>
void Player::ChangeState()
{
    //立ち
    if (!playerData.isMove && !playerData.isJump && !playerData.isRoll && !playerData.isStopRun &&
        animNumber_Now != animNum::idle)
    {
        //アニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //nowStateの中身を削除
        nowState = nullptr;
        animNumber_Now = animNum::idle;
        nowState = std::make_shared<Idle>(modelHandle,oldAnimState, nowAnimState,playerData);
       // nowState->ResetNowAnimState();
    
    }

    //走る
    if (playerData.isMove && !playerData.isRoll && playerData.isSprint &&
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
    
    //ジャンプ
    if (!playerData.isMove && playerData.isJump && !playerData.isJump_second && animNumber_Now != animNum::jump && padInput.isJump(*input))
    {
         SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

       // prevAttachIndex = nowState->GetPrevAttachIndex();
        nowState = nullptr;
        animNumber_Now = animNum::jump;
        nowState = std::make_shared<Jump>(modelHandle, oldAnimState, nowAnimState);

        playerData.isRoll = false;
    }

    //ランジャンプ
    if (playerData.isJump && playerData.isMove && !playerData.isJump_second && animNumber_Now != animNum::run_Jump && padInput.isJump(*input))
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        // prevAttachIndex = nowState->GetPrevAttachIndex();
        nowState = nullptr;
        animNumber_Now = animNum::run_Jump;
        nowState = std::make_shared<Run_Jump>(modelHandle, oldAnimState, nowAnimState);

        playerData.isRoll = false;
    }
    
    //落下中
    if (isChageState && playerData.isJump && !playerData.isRoll && animNumber_Now != animNum::falling_Idle)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;
        animNumber_Now = animNum::falling_Idle;
        nowState = std::make_shared<Falling_Idle>(modelHandle, oldAnimState, nowAnimState);
    }

    //転がる
    if (playerData.isRoll && animNumber_Now != animNum::quick_Roll && padInput.isRoll(*input))
    {
        currentJumpSpeed = 0.0f;

        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;
        animNumber_Now = animNum::quick_Roll;
        nowState = std::make_shared<Quick_Roll>(modelHandle, oldAnimState, nowAnimState);
    }

    //走り出し
    if (!playerData.isSprint && playerData.isMove && !playerData.isRoll && !playerData.isJump &&
        animNumber_Now != animNum::idle_To_Sprint)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;
        animNumber_Now = animNum::idle_To_Sprint;
        nowState = std::make_shared<Idle_To_Sprint>(modelHandle, oldAnimState, nowAnimState);
    }

    //走り終わり
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
/// アニメーション情報をセット
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
/// 上入力
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
/// 下入力
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
/// 右入力
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
/// 左入力
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
/// ジャンプ入力
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
/// ロール入力
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