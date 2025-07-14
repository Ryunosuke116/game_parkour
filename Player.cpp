#include "common.h"
#include <fstream>
#include"playerState.h"
#include "AnimTime.h"
#include "PadInput.h"
#include "Player.h"
#include "HitCheck.h"
#include "Calculation.h"

/// <summary>
/// /インストラクタ
/// </summary>
Player::Player(nlohmann::json jsonData) :
    centerPosition(VGet(0.0f, 0.0f, 0.0f)),
    footPosition(VGet(0.0f, 0.0f, 0.0f)),
    moveVec(VGet(0.0f, 0.0f, 0.0f)),
    moveDirection_now(VGet(0.0f, 0.0f, 0.0f)),
    isCalc_moveVec(false)
{
   
    std::string path = jsonData["playerPath"];

    modelHandle = MV1LoadModel(path.c_str());
    MV1SetScale(modelHandle, VGet(modelScale, modelScale, modelScale));
    collisionManager = std::make_shared<CollisionManager>();
    playerCalculation = std::make_shared<PlayerCalculation>();
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
    position = VGet(0.0f, 10.0f, 0.0f);

    MV1SetRotationXYZ(modelHandle, VGet(0, 0, 0));
   // ChangeMotion(animNum::idle, PlayAnimSpeed);

    isPush = false;
    isChangeState = false;
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
    playerData.isFalling = false;
    playerData.isHanging = false;
    playerData.isHang_to_Crouch = false;
    isCalc = false;
    isCalc_moveVec = false;
   
    oldAnimState.AttachIndex = -1;
    oldAnimState.PlayAnimSpeed = 0.0f;
    oldAnimState.PlayTime_anim = 0.0f;
    oldAnimState.TotalPlayTime_anim = 0.0f;
    nowAnimState.AttachIndex = -1;
    nowAnimState.PlayAnimSpeed = 0.0f;
    nowAnimState.PlayTime_anim = 0.0f;
    nowAnimState.TotalPlayTime_anim = 0.0f;

    animNumber_Now = animNum::idle;
    nowState = std::make_shared<Idle>(modelHandle, oldAnimState, nowAnimState, playerData);
}

/// <summary>
/// 更新
/// </summary>
void Player::Update(const VECTOR& cameraDirection,const int mapHandle)
{
    footPosition = MV1GetFramePosition(modelHandle, 2);

    //リセット
    moveVec = VGet(0.0f, 0.0f, 0.0f);

    //接地している場合リセットする
    if (playerData.isGround)
    {
        playerData.isJump          = false;
        playerData.isJump_second   = false;
        playerData.isJumpAll       = false;
        playerData.isRoll_PlayAnim = false;
        playerCalculation->SetjumpSpeed_now(0.0f);
    }

    //行動指示
    Command(cameraDirection);

    bool isFree = !playerData.isHanging
        && !playerData.isHang_to_Crouch &&
        !playerData.isFalling && !playerData.isJump && !playerData.isRoll;

    isCalc_moveVec = VSize(moveVec) != 0;

    //移動方向ベクトルが0でない場合コピー
    if (isCalc_moveVec)
    {
        targetMoveDirection = moveVec;
    }

    if (isFree)
    {
        moveDirection_now = targetMoveDirection;
    }
    else if(!playerData.isHanging && !playerData.isHang_to_Crouch)
    {
        const float speed = 0.05f;

        //todo::
        //ゆっくり最新の方向に向く
        moveDirection_now = Calculation::Leap(moveDirection_now, targetMoveDirection, speed);
    }

    //崖つかみ判定
    HangingCheck(mapHandle);

    //崖つかみ中ではない場合
    NormalMove(mapHandle);

    //崖掴み中は壁の法線に合わせて向きを決める
    HangingMove();

    //上に登る
    Hang_to_CrouchMove(mapHandle);

    //状態変更
    ChangeState();

    isChangeState = nowState->MotionUpdate(playerData);

    UpdateAngle(moveDirection_now);

    // プレイヤーのモデルの座標を更新する
    MV1SetPosition(modelHandle, position);

    if (CheckHitKey(KEY_INPUT_3))
    {
        nowFrameNumber++;
    }
    else if (CheckHitKey(KEY_INPUT_2))
    {
        nowFrameNumber--;
    }

    if (CheckHitKey(KEY_INPUT_4))
    {
        playerData.isHanging = false;
        isCalc = false;
        hangingPoint = { NULL };
    }

    //2胴体
    //0真下
    //頭 9
    //左手 65
    //右手 106
    //足もと 167
    //頭 9
    //胸 6
    //腹 4
    centerPosition = MV1GetFramePosition(modelHandle, 2);
    headPos =        MV1GetFramePosition(modelHandle, 9);
    handPos_left=    MV1GetFramePosition(modelHandle, 65);
    handPos_right =  MV1GetFramePosition(modelHandle, 106);

    handCenterPos= VAdd(handPos_left, handPos_right);
    handCenterPos = VScale(handCenterPos, 0.5f);

    VECTOR food = MV1GetFramePosition(modelHandle, 167);

    topPosition = position;
    bottomPosition = position;
    topPosition.y = topPosition.y + addTopPos;
    bottomPosition.y = bottomPosition.y + addBottomPos;

    SettingRay();
}

/// <summary>
/// 描画
/// </summary>
bool Player::Draw()
{
	MV1DrawModel(modelHandle);
    DrawSphere3D(footPosition, 2.0f, 30, GetColor(0, 0, 0),
        GetColor(255, 255, 255), FALSE);
    DrawSphere3D(handCenterPos, 2.0f, 30, GetColor(0, 0, 0),
        GetColor(0, 0, 255), FALSE);
    DrawSphere3D(playerCalculation->GetNearestResult().nearestPoint, 2.0f, 30, GetColor(0, 0, 0),
        GetColor(255, 0, 255), FALSE);

    VECTOR nowFrame = MV1GetFramePosition(modelHandle, nowFrameNumber);

    DrawSphere3D(position, 2.0f, 30, GetColor(0, 0, 0),
        GetColor(255, 0, 255), FALSE);

    VECTOR food = MV1GetFramePosition(modelHandle, 167);
    VECTOR head = MV1GetFramePosition(modelHandle, 9);
    VECTOR mune = MV1GetFramePosition(modelHandle, 6);
    VECTOR top = mune;
    top.y = top.y + (head.y - mune.y);
    VECTOR bottom = mune;
    bottom.y = bottom.y + (food.y - mune.y);

    DrawCapsule3D(topPosition, bottomPosition, radius, 30, GetColor(0, 0, 0),
        GetColor(255, 0, 0), FALSE);

   /* DrawCapsule3D(top, bottom, radius, 30, GetColor(0, 0, 0),
        GetColor(255, 0, 0), FALSE);*/

    printfDx("playerPosition.x %f\nplayerPosition.y %f\nplayerPosition.z %f\n",
        position.x, position.y, position.z);
    printfDx("frame現在数%d\n", nowFrameNumber);
    printfDx("moveSpeed_now %f\n", playerCalculation->GetmoveSpeed_now());
    printfDx("isMove %d\n", playerData.isMove);
    printfDx("isJump %d\n", playerData.isJump);
    printfDx("isJump_second %d\n", playerData.isJump_second);
    printfDx("isGround %d\n", playerData.isGround);
    printfDx("isRoll %d\n", playerData.isRoll);
    printfDx("isSprint %d\n", playerData.isSprint);
    printfDx("isStopRun %d\n", playerData.isStopRun);
    printfDx("isJumpAll %d\n", playerData.isJumpAll);
    printfDx("isFalling %d\n", playerData.isFalling);
    printfDx("isHanging %d\n", playerData.isHanging);
    printfDx("isChangeState %d\n", isChangeState);
    printfDx("animNumber_Now %d\n", animNumber_Now);
    printfDx("jumpSpeed_now %f\n", playerCalculation->GetjumpSpeed_now());
    printfDx("moveDirection_now.x %f\n", moveDirection_now.x);
    printfDx("moveDirection_now.y %f\n", moveDirection_now.y);
    printfDx("moveDirection_now.z %f\n", moveDirection_now.z);

    nowState->Draw();

    //線
   // DrawLine3D(centerPosition, footPosition, GetColor(255, 0, 0));
    DrawLine3D(topPosition, linePos_end, GetColor(255, 0, 0));
    collisionManager->Draw();
    return true;
}

void Player::Command(const VECTOR& cameraDirection)
{
    if (!playerData.isHanging && !playerData.isHang_to_Crouch)
    {
        Move(moveVec, cameraDirection);
        RollMove();
        JumpMove();
    }
}

/// <summary>
/// 掴めるところがあるか
/// </summary>
/// <param name="mapHandle"></param>
void Player::HangingCheck(const int mapHandle)
{
    if (!playerData.isHanging)
    {
        //掴めるところがあるか
        auto result_cliff = collisionManager->CliffGrabbing(mapHandle,
            topPosition, moveDirection_now, playerData.isFalling);

        playerData.isHanging = result_cliff.first;

        if (playerData.isHanging)
        {
            hangingPoint = result_cliff.second;
        }
    }
}

/// <summary>
/// 通常時の演算処理
/// </summary>
/// <param name="mapHandle"></param>
void Player::NormalMove(const int mapHandle)
{
    if ((!playerData.isHanging && !playerData.isHang_to_Crouch))
    {
        //移動量計算
        moveVec = playerCalculation->Update(moveVec, moveDirection_now,
            nowState->GetNowAnimState().PlayTime_anim,
            animNumber_Now, playerData);

        //衝突判定
        auto result = collisionManager->Update(mapHandle, position, centerPosition,
            footPosition, moveVec, moveDirection_now, radius, addTopPos,
            addBottomPos, playerData);

        playerData.isGround = result.first;

        //落下している場合
        if (playerData.isFalling)
        {
            //接地していた場合falseにする
            if (playerData.isGround)
            {
                playerData.isFalling = false;
            }
        }
        SetPos(result.second);
    }
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="input"></param>
/// <param name="moveVec"></param>
VECTOR Player::Move(VECTOR& moveVec, const VECTOR& cameraDirection)
{
    VECTOR returnPos = moveVec;
    playerData.isMove = false;
    VECTOR rightMove = VCross(cameraDirection, VGet(0.0f, 1.0f, 0.0f));

    //正規化
    rightMove = VNorm(rightMove);
    VECTOR upMove = VNorm(cameraDirection);

    upMove.y = 0.0f;
    rightMove.y = 0.0f;

    //パッド or arrowキーの入力方向で計算
    moveVec = VAdd(VScale(rightMove, -PadInput::GetJoyPad_x_left()),
        VScale(upMove, -PadInput::GetJoyPad_y_left()));

    //0でなければ正規化
    if (VSize(moveVec) != 0)
    {
        moveVec = VNorm(moveVec);
        playerData.isMove = true;
        playerData.isStopRun = true;
    }

    return returnPos;
    
}


/// <summary>
/// ジャンプ
/// </summary>
void Player::JumpMove()
{
    if (PadInput::isJump())
    {
        //一回目のジャンプ
        if (!playerData.isJump)
        {
            isPush = true;
            playerCalculation->SetjumpSpeed_now(addJumpPower);
            playerData.isJump = true;
        }
        //二段ジャンプ
        if (playerData.isJump && !isPush && !playerData.isJump_second)
        {
            isPush = true;
            playerData.isJump_second = true;
            playerCalculation->SetjumpSpeed_now(addJumpPower);
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
/// ロール
/// </summary>
void Player::RollMove()
{
    if (PadInput::isRoll() && !playerData.isRoll_PlayAnim)
    {
        playerData.isRoll = true;
    }

}

/// <summary>
/// 崖に掴まる
/// </summary>
void Player::HangingMove()
{
    if (playerData.isHanging)
    {
        if (!isCalc)
        {
            playerCalculation->Reset_move();
            moveDirection_now = playerCalculation->HangingDirection(collisionManager->GetHangingPoly(), centerPosition);
            isCalc = true;
        }

        //手の位置に角を合わせる
        //※未完全
        VECTOR addPos = playerCalculation->HangingPosition(handPos_left,
            handPos_right, playerCalculation->GetNearestResult().nearestPoint);

        position = VAdd(position, addPos);

        if (PadInput::isUp())
        {
            playerData.isHang_to_Crouch = true;
            isCalc = false;
        }

        if (PadInput::isDown())
        {
            isChangeState = true;
            playerData.isHanging = false;
            isCalc = false;
        }
    }
}

/// <summary>
/// 登り
/// </summary>
/// <param name="mapHandle"></param>
void Player::Hang_to_CrouchMove(const int mapHandle)
{
   //////////////////////////////////
   //  コード整理しろ！
   /////////////////////////////////
    if (playerData.isHang_to_Crouch && !playerData.isHanging)
    {
        //指定のフレームまでは手に合わせて座標を更新
        if (nowState->GetNowAnimState().PlayTime_anim <= 22.0f)
        {
            VECTOR addPos = playerCalculation->HangingPosition(handPos_left,
                handPos_right, playerCalculation->GetNearestResult().nearestPoint);

            position = VAdd(position, addPos);
        }
        //
        else
        {
            //上り終わった後に少しずつ前進する
            VECTOR addPos = VScale(moveDirection_now, 0.35f);
            position = VAdd(position, addPos);

            //胸の位置が床の位置をすぎたら足もとを基準に床との衝突判定をする
            VECTOR chestPos = MV1GetFramePosition(modelHandle, 6);

            VECTOR nowPos = chestPos;
            nowPos.y = position.y;

            VECTOR newPos = VAdd(nowPos, moveVec);

            //足のフレーム座標で衝突判定
            VECTOR food = MV1GetFramePosition(modelHandle, 167);

            auto result = collisionManager->GroundCollisionCheck_Hang_to_Crouch(mapHandle, nowPos, newPos, food, addTopPos, radius, addBottomPos);

            //playerの座標はフレーム座標を基準にしていないため縦だけずらす
            position.y = result.second.y;

            //上り終わったらplayerDataを初期化
            if (nowState->GetNowAnimState().PlayTime_anim >=
                nowState->GetNowAnimState().TotalPlayTime_anim - 1.0f)
            {
                playerData.isHang_to_Crouch = false;
                playerData.isMove = false;
                playerData.isJump = false;
                playerData.isRoll = false;
                playerData.isStopRun = false;
                auto result = collisionManager->GroundCollisionCheck_Hang_to_Crouch(mapHandle, nowPos, newPos, food, addTopPos, radius, addBottomPos);

                //playerData.isGround = result.first;
                position.y = result.second.y;
            }
        }
    }
}


/// <summary>
/// アニメーション変更
/// </summary>
void Player::ChangeState()
{
    //立ち
    if (!playerData.isMove && !playerData.isJump && !playerData.isRoll && !playerData.isStopRun && 
        !playerData.isHanging && !playerData.isHang_to_Crouch &&
        animNumber_Now != animNum::idle)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //nowStateの中身を削除
        nowState = nullptr;
        animNumber_Now = animNum::idle;

        //nowStateを更新
        nowState = std::make_shared<Idle>(modelHandle,oldAnimState, nowAnimState,playerData);
    }

    //走る
    if (playerData.isMove && !playerData.isRoll && playerData.isSprint &&
        playerData.isGround && animNumber_Now != animNum::run)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //nowStateの中身を削除
        nowState = nullptr;
        animNumber_Now = animNum::run;

        //nowStateを更新
        nowState = std::make_shared<Run>(modelHandle, oldAnimState,nowAnimState);
    }
    
    //ジャンプ
    if (!playerData.isMove && (playerData.isJump || playerData.isJump_second) && !playerData.isJumpAll &&
        animNumber_Now != animNum::jump && PadInput::isJump())
    {
         SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;
        animNumber_Now = animNum::jump;

        //nowStateを更新
        nowState = std::make_shared<Jump>(modelHandle, oldAnimState, nowAnimState);

        playerData.isRoll = false;

        if (playerData.isJump && playerData.isJump_second)
        {
            playerData.isJumpAll = true;
        }
    }

    //ランジャンプ
    if (playerData.isMove && (playerData.isJump || playerData.isJump_second) && !playerData.isJumpAll &&
        animNumber_Now != animNum::run_Jump && PadInput::isJump())
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
    
    //落下中
    if (isChangeState && 
        !playerData.isGround && animNumber_Now != animNum::falling_Idle)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;
        animNumber_Now = animNum::falling_Idle;
        nowState = std::make_shared<Falling_Idle>(modelHandle, oldAnimState, nowAnimState);
        playerData.isFalling = true;
        playerData.isRoll = false;
    }

    //転がる
    if (playerData.isRoll && animNumber_Now != animNum::quick_Roll && !playerData.isRoll_PlayAnim &&
        PadInput::isRoll())
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

    //崖つかみ
    if (playerData.isHanging && playerData.isFalling && 
        animNumber_Now != animNum::hanging_Idle)
    {
        SetOldAnimState(nowState->GetOldAnimState());
        SetNowAnimState(nowState->GetNowAnimState());

        nowState = nullptr;
        animNumber_Now = animNum::hanging_Idle;
        nowState = std::make_shared<Hanging_Idle>(modelHandle, oldAnimState, nowAnimState, playerData);
        playerData.isFalling = false;
    }

    //崖のぼり
    if (playerData.isHanging && playerData.isHang_to_Crouch &&
        animNumber_Now != animNum::braced_Hang_To_Crouch)
    {
        SetOldAnimState(nowState->GetOldAnimState());
        SetNowAnimState(nowState->GetNowAnimState());

        nowState = nullptr;
        animNumber_Now = animNum::braced_Hang_To_Crouch;
        nowState = std::make_shared<Braced_Hang_To_Crouch>(modelHandle, oldAnimState, nowAnimState, playerData);
        playerData.isHanging = false;
    }
}

void Player::Reset()
{
   
}

/// <summary>
/// アニメーション情報をセット
/// </summary>
/// <param name="AnimState"></param>
void Player::SetOldAnimState(PlayerStateActionBase::AnimState animState)
{
    oldAnimState.AttachIndex = animState.AttachIndex;
    oldAnimState.PlayAnimSpeed = animState.PlayAnimSpeed;
    oldAnimState.PlayTime_anim = animState.PlayTime_anim;
    oldAnimState.TotalPlayTime_anim = animState.TotalPlayTime_anim;
}

void Player::SetNowAnimState(PlayerStateActionBase::AnimState animState)
{
    nowAnimState.AttachIndex = animState.AttachIndex;
    nowAnimState.PlayAnimSpeed = animState.PlayAnimSpeed;
    nowAnimState.PlayTime_anim = animState.PlayTime_anim;
    nowAnimState.TotalPlayTime_anim = animState.TotalPlayTime_anim;
}

void Player::SettingRay()
{
    linePos_end = VAdd(position, VScale(moveDirection_now, 10.0f));
    linePos_end.y = topPosition.y - 1.0f;
  
}

void Player::Update() {};