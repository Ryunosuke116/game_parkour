#include "common.h"
#include <fstream>
#include"playerState.h"
#include "AnimTime.h"
#include "Json.h"
#include "PadInput.h"
#include "Player.h"
#include "HitCheck.h"
#include "Calclation.h"
#include "nlohmann/json.hpp"

/// <summary>
/// /インストラクタ
/// </summary>
Player::Player() :
    centerPosition(VGet(0.0f, 0.0f, 0.0f)),
    footPosition(VGet(0.0f, 0.0f, 0.0f)),
    moveVec(VGet(0.0f, 0.0f, 0.0f)),
    moveVec_memory(VGet(0.0f, 0.0f, 0.0f))
{
    JsonFile::UnInitialize();
    JsonFile::Initialize("Json/player.json");

    std::string path = JsonFile::GetJson()["playerPath"];

    modelHandle = MV1LoadModel(path.c_str());
    MV1SetScale(modelHandle, VGet(modelScale, modelScale, modelScale));
    collisionManager = std::make_shared<CollisionManager>();
    playerCalclation = std::make_shared<PlayerCalclation>();
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
    playerData.isFalling = false;
    playerData.isHangring = false;
    isCalc = false;
   
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
void Player::Update(const VECTOR& cameraDirection,const int mapHandle)
{
    //リセット
    moveVec = VGet(0.0f, 0.0f, 0.0f);

    //接地している場合リセットする
    if (playerData.isGround)
    {
        playerData.isJump = false;
        playerData.isJump_second = false;
        playerData.isJumpAll = false;
        playerData.isRoll_PlayAnim = false;
        playerCalclation->SetCurrentJumpSpeed(0.0f);
    }
   
    //行動指示
    Move(moveVec, cameraDirection);
    RollMove();
    JumpMove();

    //移動方向ベクトルが0でない場合コピー
    if (VSize(moveVec) != 0 && !playerData.isHangring)
    {
        targetMoveDirection = moveVec;
    }

    //崖つかみ判定
    if (!playerData.isHangring)
    {
        //掴めるところがあるか
        auto result_cliff = collisionManager->CliffGrabbing(mapHandle, topPosition, targetMoveDirection, playerData.isFalling);
        playerData.isHangring = result_cliff.first;


        if (playerData.isHangring)
        {
            hangringPoint = result_cliff.second;
        }
    }

    //状態変更
    ChangeState();

    
    if (CheckHitKey(KEY_INPUT_4))
    {
        playerData.isHangring = false;
        isCalc = false;
        hangringPoint = { NULL };
    }

    //崖つかみ中ではない場合
    if(!playerData.isHangring)
    {
        moveVec = playerCalclation->Update(moveVec, targetMoveDirection,
            nowState->GetNowAnimState().PlayTime_anim,
            animNumber_Now, playerData);

        //衝突判定
        auto result = collisionManager->Update(mapHandle, position,centerPosition,
            moveVec, targetMoveDirection, radius, addTopPos,
            addBottomPos, playerData.isJump, playerData.isFalling);

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

    //崖掴み中は壁の法線に合わせて向きを決める
    if (playerData.isHangring)
    {
        if (!isCalc)
        {
            targetMoveDirection = playerCalclation->HangringDirection(collisionManager->GetHangringPoly(), centerPosition);
            isCalc = true;
        }

        //手の位置に角を合わせる
        //※未完全
        VECTOR addPos = playerCalclation->HangringPosition(handPos_left,
            handPos_right, playerCalclation->GetNearestResult().nearestPoint);
      
        position = VAdd(position, addPos);
    }

    isChageState = nowState->MotionUpdate(playerData);

    UpdateAngle(targetMoveDirection);

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
        playerData.isHangring = false;
    }

    //2胴体
    //0真下
    //頭 9
    //左手 65
    //右手 106
    centerPosition = MV1GetFramePosition(modelHandle, 2);
    footPosition = MV1GetFramePosition(modelHandle, 0);
    headPos = MV1GetFramePosition(modelHandle, 9);
    handPos_left= MV1GetFramePosition(modelHandle, 65);
    handPos_right= MV1GetFramePosition(modelHandle, 106);
    handCenterPos= VAdd(handPos_left, handPos_right);
    handCenterPos = VScale(handCenterPos, 0.5f);

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
    DrawSphere3D(centerPosition, 1.0f, 30, GetColor(0, 0, 0),
        GetColor(255, 0, 0), FALSE);
    DrawSphere3D(handCenterPos, 2.0f, 30, GetColor(0, 0, 0),
        GetColor(0, 0, 255), FALSE);
    DrawSphere3D(nearestPoint, 2.0f, 30, GetColor(0, 0, 0),
        GetColor(255, 0, 255), FALSE);
   /* DrawCapsule3D(topPosition, bottomPosition, radius, 30, GetColor(0, 0, 0),
        GetColor(255, 0, 0), FALSE);*/

    printfDx("playerPosition.x %f\nplayerPosition.y %f\nplayerPosition.z %f\n",
        position.x, position.y, position.z);
    //printfDx("frame現在数%d\n", nowFrameNumber);
    printfDx("nowMoveSpeed %f\n", playerCalclation->GetNowMoveSpeed());
    printfDx("isMove %d\n", playerData.isMove);
    printfDx("isJump %d\n", playerData.isJump);
    printfDx("isJump_second %d\n", playerData.isJump_second);
    printfDx("isGround %d\n", playerData.isGround);
    printfDx("isRoll %d\n", playerData.isRoll);
    printfDx("isSprint %d\n", playerData.isSprint);
    printfDx("isStopRun %d\n", playerData.isStopRun);
    printfDx("isJumpAll %d\n", playerData.isJumpAll);
    printfDx("isFalling %d\n", playerData.isFalling);
    printfDx("isHangring %d\n", playerData.isHangring);
    printfDx("isChageState %d\n", isChageState);
    printfDx("animNumber_Now %d\n", animNumber_Now);
    printfDx("currentJumpSpeed %f\n", playerCalclation->GetCurrentJumpSpeed());
    nowState->Draw();

    //線
   // DrawLine3D(centerPosition, footPosition, GetColor(255, 0, 0));
    DrawLine3D(topPosition, linePos_end, GetColor(255, 0, 0));
    collisionManager->Draw();
    return true;
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

    ////上入力されたとき
    //if (PadInput::isUp())
    //{
    //    if (animNumber_Now != animNum::run && !playerData.isJump)
    //    {
    //       // ChangeMotion(animNum::run, PlayAnimSpeed);
    //    }
    //    moveVec = VAdd(moveVec, upMove);
    //    playerData.isMove = true;
    //    playerData.isStopRun = true;
    //}

    ////下入力されたとき
    //if (PadInput::isDown())
    //{
    //    if (animNumber_Now != animNum::run && !playerData.isJump)
    //    {
    //       // ChangeMotion(animNum::run, PlayAnimSpeed);
    //    }
    //    moveVec = VAdd(moveVec, VScale(upMove, -1.0f));
    //    playerData.isMove = true;
    //    playerData.isStopRun = true;
    //}

    ////左入力されたとき
    //if (PadInput::isLeft())
    //{
    //    if (animNumber_Now != animNum::run && !playerData.isJump)
    //    {
    //       // ChangeMotion(animNum::run, PlayAnimSpeed);
    //    }
    //    moveVec = VAdd(moveVec, rightMove);
    //    playerData.isMove = true;
    //    playerData.isStopRun = true;
    //}

    ////右入力されたとき
    //if (PadInput::isRight())
    //{
    //    if (animNumber_Now != animNum::run && !playerData.isJump)
    //    {
    //        //ChangeMotion(animNum::run, PlayAnimSpeed);
    //    }
    //    moveVec = VAdd(moveVec, VScale(rightMove, -1.0f));
    //    playerData.isMove = true;
    //    playerData.isStopRun = true;
    //}

 /*   moveVec = VGet(PadInput::GetJoyPad_x_left(),
        0.0f, -PadInput::GetJoyPad_y_left());*/
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
            playerCalclation->SetCurrentJumpSpeed(addJumpPower);
            playerData.isJump = true;
        }
        //二段ジャンプ
        if (playerData.isJump && !isPush && !playerData.isJump_second)
        {
            isPush = true;
            playerData.isJump_second = true;
            playerCalclation->SetCurrentJumpSpeed(addJumpPower);
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

void Player::HangringMove()
{
    if (playerData.isHangring)
    {

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
    if (isChageState && playerData.isJump && !playerData.isRoll
        && animNumber_Now != animNum::falling_Idle)
    {
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        nowState = nullptr;
        animNumber_Now = animNum::falling_Idle;
        nowState = std::make_shared<Falling_Idle>(modelHandle, oldAnimState, nowAnimState);
        playerData.isFalling = true;
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
    if (playerData.isHangring && playerData.isFalling && 
        animNumber_Now != animNum::hangring_Idle)
    {
        SetOldAnimState(nowState->GetOldAnimState());
        SetNowAnimState(nowState->GetNowAnimState());

        nowState = nullptr;
        animNumber_Now = animNum::hangring_Idle;
        nowState = std::make_shared<Hangring_Idle>(modelHandle, oldAnimState, nowAnimState, playerData);
        playerData.isFalling = false;
    }
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
    linePos_end = VAdd(position, VScale(targetMoveDirection, 10.0f));
    linePos_end.y = topPosition.y - 1.0f;
  
}

void Player::Update() {};