#include "common.h"
#include <fstream>
#include"playerState.h"
#include "AnimTime.h"
#include "PadInput.h"
#include "Player.h"
#include "HitCheck.h"
#include "Calculation.h"
#include "BaseObject.h"



/// <summary>
/// /インストラクタ
/// </summary>
Player::Player(nlohmann::json jsonData) :
    centerPosition(VGet(0.0f, 0.0f, 0.0f)),
    moveVec(VGet(0.0f, 0.0f, 0.0f)),
    moveDirection_now(VGet(0.0f, 0.0f, 0.0f)),
    isCalc_moveVec(false)
{
   
    std::string path = jsonData["playerPath"];

    modelHandle = MV1LoadModel(path.c_str());
    MV1SetScale(modelHandle, VGet(modelScale, modelScale, modelScale));
    collisionManager = std::make_shared<CollisionManager>();
    playerCalculation = std::make_shared<PlayerCalculation>();
    animationChecger = std::make_shared<AnimationChanger>();
    positionData.footPosition = VGet(0.0f, 0.0f, 0.0f);
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
    isChange_falling = false;
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
    playerData.isSlip = false;
    playerData.isRun = false;
    playerData.isIdle = true;
    playerData.isRollFinished = false;
    playerData.isRun_wall = false;
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
    degree_pad_now = 0.0f;
    padInput_now = VGet(0.0f, 0.0f, 0.0f);

    animNumber_Now = animNum::idle;
    nowState = std::make_shared<Idle>(modelHandle, oldAnimState, nowAnimState, playerData);
}

/// <summary>
/// 更新
/// </summary>
void Player::Update(const VECTOR& cameraDirection, 
    const std::vector<std::shared_ptr<BaseObject>>& fieldObjects)
{
    positionData.footPosition = MV1GetFramePosition(modelHandle, 2);

    //リセット
    moveVec = VGet(0.0f, 0.0f, 0.0f);

    //接地している場合リセットする
    if (playerData.isGround)
    {
        playerData.isJump          = false;
        playerData.isJump_second   = false;
        playerData.isJumpAll       = false;
        playerData.isRollFinished  = false;
        if (!playerData.isRoll_PlayAnim)
        {
            playerData.isRoll = false;
        }

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
        moveDirection_now = Calculation::Leap(moveDirection_now, 
            targetMoveDirection, 0.15f);
    }
    else if(!playerData.isHanging && !playerData.isHang_to_Crouch)
    {
        const float speed = 0.03f;

        //todo::
        //ゆっくり最新の方向に向く
        moveDirection_now = Calculation::Leap(moveDirection_now,
            targetMoveDirection, speed);
    }

    //崖つかみ判定
    HangingCheck(fieldObjects);

    //state別更新処理
    StateUpdate(fieldObjects);

    //スティックが即座に反対方向に向いた場合slipをtrue
    float radian_new = atan2f(moveDirection_now.x, moveDirection_now.z);

    float degree_now = Calculation::radToDeg(angle);
    float degree_new = Calculation::radToDeg(radian_new);

    degree_difference = degree_new - degree_now;
    //角度差を-180～180に正規化
    while (degree_difference > 180.0f) degree_difference -= 360.0f;
    while (degree_difference < -180.0f) degree_difference += 360.0f;

    //絶対値で調べる
    if (abs(degree_difference) >= 170.0f &&
        playerData.isGround && playerData.isRun && !playerData.isRoll_PlayAnim &&
        playerCalculation->GetmoveSpeed_now() >= (playerCalculation->GetmoveSpeed_max() - 0.4f))
    {
        playerData.isSlip = true;
    }

    if (!playerData.isSlip)
    {
        UpdateAngle(moveDirection_now, playerData.isTurn_right);
    }
    
    //状態変更
    animationChecger->ChangeState(modelHandle, isChange_falling, playerData,
        nowState);

    isChange_falling = nowState->MotionUpdate(playerData);

    // プレイヤーのモデルの座標を更新する
    MV1SetPosition(modelHandle, position);

    if (CheckHitKey(KEY_INPUT_3))
    {
        x++;
    }
    else if (CheckHitKey(KEY_INPUT_2))
    {
        x--;
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

    VECTOR foot = MV1GetFramePosition(modelHandle, 167);

    topPosition = position;
    bottomPosition = position;
    topPosition.y = topPosition.y + positionData.addTopPos;
    bottomPosition.y = bottomPosition.y + positionData.addBottomPos;

    SettingRay();
}

/// <summary>
/// 描画
/// </summary>
bool Player::Draw()
{
	MV1DrawModel(modelHandle);
    DrawSphere3D(positionData.footPosition, 2.0f, 30, GetColor(0, 0, 0),
        GetColor(255, 255, 255), FALSE);
    DrawSphere3D(handCenterPos, 2.0f, 30, GetColor(0, 0, 0),
        GetColor(0, 0, 255), FALSE);
    DrawSphere3D(playerCalculation->GetNearestResult().nearestPoint, 2.0f, 30, GetColor(0, 0, 0),
        GetColor(255, 0, 255), FALSE);

    VECTOR nowFrame = MV1GetFramePosition(modelHandle, nowFrameNumber);

    DrawSphere3D(position, 2.0f, 30, GetColor(0, 0, 0),
        GetColor(255, 0, 255), FALSE);

    VECTOR foot = MV1GetFramePosition(modelHandle, 167);
    VECTOR head = MV1GetFramePosition(modelHandle, 9);
    VECTOR mune = MV1GetFramePosition(modelHandle, 6);
    VECTOR top = mune;
    top.y = top.y + (head.y - mune.y);
    VECTOR bottom = mune;
    bottom.y = bottom.y + (foot.y - mune.y);

    /*DrawCapsule3D(topPosition, bottomPosition, positionData.radius, 30, GetColor(0, 0, 0),
        GetColor(255, 0, 0), FALSE);*/

   /* DrawCapsule3D(top, bottom, radius, 30, GetColor(0, 0, 0),
        GetColor(255, 0, 0), FALSE);*/

    printfDx("playerPosition.x %f\nplayerPosition.y %f\nplayerPosition.z %f\n",
        position.x, position.y, position.z);
    printfDx("frame現在数%d\n", nowFrameNumber);
    printfDx("moveSpeed_now %f\n", playerCalculation->GetmoveSpeed_now());
    printfDx("isIdle %d\n", playerData.isIdle);
    printfDx("isMove %d\n", playerData.isMove);
    printfDx("isRun %d\n", playerData.isRun);
    printfDx("isSlip %d\n", playerData.isSlip);
    printfDx("isJump %d\n", playerData.isJump);
    printfDx("isJump_second %d\n", playerData.isJump_second);
    printfDx("isGround %d\n", playerData.isGround);
    printfDx("isRoll %d\n", playerData.isRoll);
    printfDx("isRoll_PlayAnim %d\n", playerData.isRoll_PlayAnim);
    printfDx("isSprint %d\n", playerData.isSprint);
    printfDx("isStopRun %d\n", playerData.isStopRun);
    printfDx("isJumpAll %d\n", playerData.isJumpAll);
    printfDx("isFalling %d\n", playerData.isFalling);
    printfDx("isHanging %d\n", playerData.isHanging);
    printfDx("isTurn_right %d\n", playerData.isTurn_right);
    printfDx("isRun_wall %d\n", playerData.isRun_wall);
    printfDx("isChange_falling %d\n", isChange_falling);
    printfDx("animNumber_Now %d\n", animNumber_Now);
    printfDx("jumpSpeed_now %f\n", playerCalculation->GetjumpSpeed_now());
    printfDx("moveVec_normal.x %f\n", moveVec_normal.x);
    printfDx("moveVec_normal.y %f\n", moveVec_normal.y);
    printfDx("moveVec_normal.z %f\n", moveVec_normal.z);
    printfDx("JoyPad_x_left %f\n", PadInput::GetJoyPad_x_left());
    printfDx("JoyPad_y_left %f\n", PadInput::GetJoyPad_y_left());
    printfDx("degree_pad_now %f\n", degree_pad_now);
    printfDx("degree_pad_wall_difference %f\n", degree_pad_wall_difference);
    printfDx("degree_difference %f\n", degree_difference);
    printfDx("x %f\n", x);

    nowState->Draw();

    //線
   // DrawLine3D(centerPosition, footPosition, GetColor(255, 0, 0));
    //DrawLine3D(topPosition, linePos_end, GetColor(255, 0, 0));
    collisionManager->Draw();
    return true;
}

void Player::StateUpdate(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects)
{
    //崖つかみ中ではない場合
    if ((!playerData.isHanging && !playerData.isHang_to_Crouch) && 
        !playerData.isRun_wall)
    {
        NormalMove(fieldObjects);
    }
    
    //崖つかみ中ではない場合
    if (playerData.isRun_wall && 
        (!playerData.isHanging && !playerData.isHang_to_Crouch))
    {
        WallRunUpdate(fieldObjects);
    }

    //崖掴み中は壁の法線に合わせて向きを決める
    if (playerData.isHanging)
    {
        HangingMove();
    }

    //上に登る
    if (playerData.isHang_to_Crouch && !playerData.isHanging)
    {
        Hang_to_CrouchMove(fieldObjects);
    }

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
void Player::HangingCheck(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects)
{
    if (!playerData.isHanging)
    {
        //掴めるところがあるか
        auto result_cliff = collisionManager->CliffGrabbing(fieldObjects,
            topPosition, moveDirection_now, playerData.isFalling);

        playerData.isHanging = result_cliff.first;

        //掴める場合掴む座標を取得
        if (playerData.isHanging)
        {
            hangingPoint = result_cliff.second;
            playerData.isRun_wall = false;
        }
    }
}

/// <summary>
/// 通常時の演算処理
/// </summary>
/// <param name="mapHandle"></param>
void Player::NormalMove(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects)
{
    if (playerCalculation->GetmoveSpeed_now() <= 0.0f)
    {
        playerData.isSlip = false;
    }

    //移動量計算
    moveVec = playerCalculation->Update(moveVec, moveDirection_now,
        nowState->GetNowAnimState().PlayTime_anim,
        animNumber_Now, playerData);

    //衝突判定
    auto [isHitGround, isHitWall, isHitWall_normal, newPos, objectTag] = collisionManager->Update(fieldObjects, position,
        moveVec, positionData, playerData);

    playerData.isGround = isHitGround;
    playerData.isHitWall = isHitWall;

    //落下中に壁に当たっていたら壁走り可能に
    if (playerData.isFalling && playerData.isHitWall)
    {
        //スティックが即座に反対方向に向いた場合slipをtrue
        //radian計算
        radian_wall = atan2f(-isHitWall_normal.x, -isHitWall_normal.z);
        float radian_pad = atan2f(moveVec_normal.x, moveVec_normal.z);
        
        //度数計算
        degree_pad_now = Calculation::radToDeg(radian_pad);
        float degree_wall = Calculation::radToDeg(radian_wall);

        //スティック入力と壁の角度の差を求める
        degree_pad_wall_difference = degree_pad_now - degree_wall;

        //壁の法線ベクトルを利用して壁走りするかどうか
        if (abs(degree_pad_wall_difference) <= entryDegree_wallRun &&
            !playerData.isRun_wall)
        {
            //ロールアクションとジャンプをできないように
            playerData.isRun_wall = true;
            playerData.isRoll_PlayAnim = true;

            playerData.isJump_second = false;
            playerData.isJumpAll = false;
            isChange_falling = false;
            playerCalculation->SetjumpSpeed_now(addJumpPower);
            moveDirection_now = VScale(isHitWall_normal, -1.0f);

            x = run_wall_rotate_x;
        }
    }

    //落下している場合
    if (playerData.isFalling)
    {
        //接地していた場合falseにする
        if (playerData.isGround)
        {
            playerData.isFalling = false;
        }
    }

    //対象のオブジェクトの移動量を加算
    for (const auto& fieldObject : fieldObjects)
    {
        if (objectTag == fieldObject->GetTag())
        {
            newPos = VAdd(newPos, fieldObject->GetPos_difference());
        }
    }

    SetPos(newPos);
}

/// <summary>
/// 壁走り処理
/// </summary>
/// <param name="fieldObjects"></param>
void Player::WallRunUpdate(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects)
{  
    moveVec = playerCalculation->Run_Wall(moveVec,playerData);

    auto [isHitGround, isHitWall, isHitWall_normal, newPos,
        objectTag] = collisionManager->Update(fieldObjects,
            position, moveVec, positionData, playerData);
    SetPos(newPos);

    if (PadInput::isDown())
    {
        playerData.isRun_wall = false;
        playerCalculation->Reset_run_wall();
        x = 0.0f;
    }

    if (playerData.isJump_second)
    {
        playerData.isRun_wall = false;
        moveDirection_now = VScale(moveDirection_now, -1.0f);
        targetMoveDirection = moveDirection_now;
        x = 0.0f;
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
        moveVec_normal = moveVec;
        playerData.isMove = true;
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
        if (!playerData.isJump && !isPush)
        {
            isPush = true;
            playerCalculation->SetjumpSpeed_now(addJumpPower);
            playerData.isJump = true;
            if (!playerData.isMove)
            {
                playerData.isJump_PlayAnim = true;
            }
            else
            {
                playerData.isJump_run_playAnim = true;
            }
        }
        //二段ジャンプ
        if (playerData.isJump && !isPush && !playerData.isJump_second)
        {
            isPush = true;
            playerData.isJump_second = true;
            playerData.isJumpAll = true;
            playerCalculation->SetjumpSpeed_now(addJumpPower);
            if (!playerData.isMove)
            {
                playerData.isJump_PlayAnim = true;
            }
            else
            {
                playerData.isJump_run_playAnim = true;
            }

            moveDirection_now = targetMoveDirection;
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
    if (PadInput::isRoll() && !playerData.isRoll_PlayAnim &&
        !playerData.isRollFinished)
    {
        playerData.isRoll = true;
    }

}

/// <summary>
/// 崖に掴まる
/// </summary>
void Player::HangingMove()
{
    if (!isCalc)
    {
        playerCalculation->Reset_move();
        moveDirection_now = playerCalculation->HangingDirection(collisionManager->GetHangingPoly(),
            centerPosition);
        isCalc = true;
    }

    //手の位置に角を合わせる
    //※未完全
    VECTOR addPos = playerCalculation->HangingPosition(handPos_left,
        handPos_right, playerCalculation->GetNearestResult().nearestPoint);

    position = VAdd(position, addPos);

    //上に登る
    if (PadInput::isUp())
    {
        playerData.isHang_to_Crouch = true;
        isCalc = false;
    }

    //降りる
    if (PadInput::isDown())
    {
        isChange_falling = true;
        playerData.isHanging = false;
        isCalc = false;
    }
}

/// <summary>
/// 登り
/// </summary>
/// <param name="mapHandle"></param>
void Player::Hang_to_CrouchMove(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects)
{
   //////////////////////////////////
   //  コード整理しろ！
   /////////////////////////////////
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
        VECTOR foot = MV1GetFramePosition(modelHandle, 167);

        auto result = collisionManager->GroundCollisionCheck_Hang_to_Crouch(fieldObjects, nowPos, newPos, foot, positionData);

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
            auto result = collisionManager->GroundCollisionCheck_Hang_to_Crouch(fieldObjects, nowPos, newPos, foot, positionData);

            //playerData.isGround = result.first;
            position.y = result.second.y;
        }
    }
}




void Player::SettingRay()
{
    linePos_end = VAdd(position, VScale(moveDirection_now, 10.0f));
    linePos_end.y = topPosition.y - 1.0f;
  
}


void Player::Update() {};