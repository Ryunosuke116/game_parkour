#include "common.h"
#include <fstream>
#include"playerState.h"
#include "AnimTime.h"
#include "PadInput.h"
#include "EffectManager.h"
#include "Player.h"
#include "HitCheck.h"
#include "Calculation.h"
#include "BaseObject.h"
#include "DebugDrawer.h"
#include "Camera.h"
#include "JsonManager.h"
#include "WorldSubSystem.h"
#include "CollisionObjectManager.h"
#include "SubSystemManager.h"

/// <summary>
/// /インストラクタ
/// </summary>
Player::Player() :
    BaseChara(),
    start_walkTime(-1),
    nowMoveDirection(VGet(0.0f, 0.0f, 0.0f)),
    moveVec_normal(VGet(0.0f,0.0f,0.0f)),
    faceDirection(VGet(0.0f,0.0f,0.0f)),
    isCalcMoveVec(false),
    playerData({false})
{
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{

}

void Player::Create()
{
    const std::string jsonName = "player";
    Load(JsonManager::GetInstance().GetJsons(jsonName));
    playerCalculation = std::make_shared<PlayerCalculation>();
    animationChanger = std::make_shared<AnimationChanger>();
    MV1SetScale(modelHandle, VGet(modelScale, modelScale, modelScale));
}

/// <summary>
/// 読み込み
/// </summary>
/// <param name="jsonData"></param>
void Player::Load(const nlohmann::json& jsonData)
{
    std::string path = jsonData["playerPath"];
    modelHandle = MV1LoadModel(path.c_str());
}

/// <summary>
/// 初期化
/// </summary>
void Player::Initialize()
{
    const VECTOR initPosition = VGet(3.02443838f, 9.00285912f, -1215.93481f);

    const VECTOR pos = VGet(-1663.0f, 688.5f, 788.0f);

    position = pos;
    targetMoveDirection = VGet(0.0f, 0.0f, 0.0f);
    radian = 0.0f;
    rotate_x = 0.0f;
    start_walkTime = 0.0f;

    MV1SetPosition(modelHandle, position);

    isPush = false;
    isChange_falling = false;

    playerData.isIdle = false;
    playerData.isGround = true;
    playerData.isJump = false;
    playerData.isJumpSecond = false;
    playerData.isJumpAll = false;
    playerData.isWalljump = false;
    playerData.isMove = false;
    playerData.isWalk = true;
    playerData.IsPushRT = false;
    playerData.isSprint = false;
    playerData.isStopRun = false;
    playerData.isUseRoll = false;
    playerData.isJumpPlayAnim = false;
    playerData.isFalling = false;
    playerData.isHanging = false;
    playerData.isHanging_now = false;
    playerData.isUseHanging = true;
    playerData.isHangToCrouch = false;
    playerData.isRun = false;
    playerData.isRunWall = false;
    playerData.isUseWallJump = true;
    playerData.isDash = false;
    playerData.isWallClimb = false;
    isCalc = false;
    isCalcMoveVec = false;
    isCollisionCheck = true;
   
    coinCount = 0;
    degree_pad_now = 0.0f;
    effectTimer = 0.0f;
    padInput_now = VGet(0.0f, 0.0f, 0.0f);
    nowMoveDirection = VGet(0.0f, 0.0f, 0.0f);

    MV1SetRotationXYZ(modelHandle, VGet(rotate_x * DX_PI_F / 180.0f, radian + DX_PI_F, 0.0f));

    animationChanger->Initialize(animNum::walk, modelHandle, nowState, playerData, *this);
}

/// <summary>
/// 更新
/// </summary>
void Player::Update()
{
     //positionData更新
    CollisionUpdate();

    //エフェクトマネージャーのポインタを参照
    std::shared_ptr<EffectManager> effectManager = SubSystemManager::GetInstance().GetSubSystem<EffectManager>().lock();

    //リセット
    moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //接地中であればロールアクションを使えるように
    if (playerData.isGround && !playerData.IsPushRT)
    {
        playerData.isUseRoll = false;
    }

    //stateに応じた挙動処理
    auto [moveDirection_new, newData] = nowState->Update(
        WorldSubSystem::GetInstance().GetSubSystem<Camera>()->GetCameraDirection(),
        WorldSubSystem::GetInstance().GetSubSystem<CollisionObjectManager>()->GetCollisionObjects(),
        *this);

    moveDirection = moveDirection_new;

    playerData = newData;

    MoveDirectionUpdate();

    //キャラクターの向く方向を更新
    UpdateAngle(faceDirection);
   
    //状態変更
    ChangeState();

    nowState->MotionUpdate(playerData);

    //move計算
    velocity = playerCalculation->Update(
        nowMoveDirection,
        nowState->GetNowAnimState().PlayTime_anim,
        animationChanger->GetAnimNumber_now(),
        playerData);

    if (playerData.isRun)
    {
        effectTimer++;
        if (effectTimer >= 10.0f)
        {
            effectManager->PlayEffect("foot_smoke");
            effectManager->SetPosition(position,"foot_smoke");
            effectTimer = 0.0f;
        }
    }
    else
    {
        positionData.rayBottomPosition = position;
        positionData.rayBottomPosition.y -= 0.1f;
    }

    ///////////////////////////////////////
    //  デバッグ用
    //////////////////////////////////////
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
    }

    float radian_pad = atan2f(-PadInput::GetJoyPad_x_left(), -PadInput::GetJoyPad_y_left());

    //度数計算
    degree_pad_now = Calculation::radToDeg(radian_pad);

    DebugUpdate();
    
    nowState->Draw();
    //2胴体
    //0真下
    //頭 9
    //左手 65
    //右手 106
    //足もと 167
    //頭 9
    //胸 6
    //腹 4
}

/// <summary>
/// ゲームシーンのスタート時の更新処理
/// </summary>
/// <param name="timer"></param>
void Player::Update_start(const float& timer)
{
    velocity = VGet(0.0f, 0.0f, 0.5f);

    if (timer <= 40.0f)
    {
        position = VAdd(position, velocity);
    }
    else
    {
        nowState->SetIsChangeState(true);
        playerData.isIdle = true;
        //状態変更
        ChangeState();
    }

    nowState->MotionUpdate(playerData);
}

/// <summary>
/// ゲームシーン終了時の更新処理
/// </summary>
/// <param name="timer"></param>
void Player::Update_finish(const float& timer)
{
    nowState->SetIsChangeState(true);
    playerData.isIdle = true;
    //状態変更
    ChangeState();

    nowState->MotionUpdate(playerData);
}

/// <summary>
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void Player::ResultCreate()
{
    Create();
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void Player::ResultInitialize()
{
    const VECTOR initPosition = VGet(0.0f, 9.00285912f, -1205.93481f);

    position = initPosition;
    radian = 0.0f;

    MV1SetPosition(modelHandle, position);

    MV1SetRotationXYZ(modelHandle, VGet(rotate_x * DX_PI_F / 180.0f, radian + DX_PI_F, 0.0f));

    animationChanger->ResultInitialize(animNum::idle, modelHandle, nowState, playerData, *this);
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void Player::ResultUpdate()
{
    nowState->MotionUpdate(playerData);
}

void Player::MoveDirectionUpdate()
{
    const bool isAction = !playerData.isHanging &&
        !playerData.isHangToCrouch &&
        !playerData.isFalling &&
        !playerData.isJump &&
        !playerData.IsPushRT;

    isCalcMoveVec = VSize(moveDirection) != 0;

    //移動方向ベクトルが0でない場合代入
    if (isCalcMoveVec)
    {
        targetMoveDirection = moveDirection;
    }

    //通常時は進行方向にすぐ向くように
    if (isAction)
    {
        const float leapSpeed = 0.15f;
        nowMoveDirection = Calculation::Leap(
            nowMoveDirection,
            targetMoveDirection,
            leapSpeed);
        nowMoveDirection.y = 0.0f;
    }
    //特定のアクション時は移動方向を変えられないように
    else if (!playerData.isHanging &&
        !playerData.isHangToCrouch &&
        !playerData.isWalljump)
    {
        const float speed = 0.03f;

        //ゆっくり最新の方向に向く
        nowMoveDirection = Calculation::Leap(
            nowMoveDirection,
            targetMoveDirection, 
            speed);
        nowMoveDirection.y = 0.0f;
    }

    if (!playerData.isRunWall && 
        nowMoveDirection.y == 0.0f)
    {
        faceDirection = nowMoveDirection;
    }
}

void Player::ChangeState()
{
    if (nowState->GetIsChangeState())
    {
        //終了時に調整
        nowState->Exit(playerData);

        nowState = std::move(animationChanger->ChangeState(modelHandle, *this, playerData, nowState));
    }
}

/// <summary>
/// コリジョンデータの更新
/// </summary>
void Player::CollisionUpdate()
{
    bool isReverse = false;
    //positionData更新
    
    const int left = MV1SearchFrame(modelHandle, "mixamorig:LeftHandIndex4");
    const int right = MV1SearchFrame(modelHandle, "mixamorig:RightHandMiddle4_end");
    const int head = 7;
    const float rightAngle = 90.0f;
    const VECTOR verticalShaft = VGet(0.0f, 1.0f, 0.0f);
    const VECTOR rotatePosition = VAdd(position, faceDirection);
    const VECTOR addCenterPosition = VGet(0.0f, 10.0f, 0.0f);       //キャラの足もとから中心までの差

    playerCalculation->SetHandPos_left(MV1GetFramePosition(modelHandle, left));
    playerCalculation->SetHandPos_right(MV1GetFramePosition(modelHandle, right));
    positionData.centerPosition = VAdd(position, addCenterPosition);
    positionData.oldPosition = position;

    //ray
    positionData.rayTopPosition = VGet(position.x, positionData.centerPosition.y + height, position.z);
    positionData.rayBottomPosition = position;
    
    positionData.rayTopPosition.x = positionData.rayBottomPosition.x;
    positionData.rayTopPosition.z = positionData.rayBottomPosition.z;
    positionData.rayBottomPosition.y -= 0.1f;
     
    //投影で歩くためのごまかし
    if (playerData.isRun && !playerData.isJump)
    {
        positionData.rayBottomPosition.y -= playerCalculation->GetMoveSpeed_now();
    }

    //キャラの横軸を求める
    positionData.sideShaft = Calculation::RotateLineSegment(
        position,
        rotatePosition,
        verticalShaft,
        rightAngle);

    //カプセル
    positionData.capsuleTopPosition = VGet(
        positionData.centerPosition.x, 
        positionData.centerPosition.y + height,
        positionData.centerPosition.z);

    positionData.capsuleBottomPosition = position;
    
    //調整
    //カプセル
    positionData.capsuleTopPosition.x = positionData.capsuleBottomPosition.x;
    positionData.capsuleTopPosition.z = positionData.capsuleBottomPosition.z;
    positionData.capsuleBottomPosition.y += radius;
    positionData.capsuleTopPosition.y -= radius;

    //bottomPosよりも下にいかないように
    //capsule
    if (positionData.capsuleTopPosition.y <= positionData.capsuleBottomPosition.y)
    {
        positionData.capsuleTopPosition.y = positionData.capsuleBottomPosition.y;
    }

    const VECTOR sideDirection = VNorm(VSub(position, positionData.sideShaft));
    
    //縦の軸の線分を回転させる
    if (playerData.isRunWall)
    {
        positionData.capsuleTopPosition = Calculation::RotateLineSegment(
            positionData.capsuleBottomPosition,
            positionData.capsuleTopPosition,
            sideDirection,
            rotate_x);

        positionData.centerPosition = Calculation::RotateLineSegment(
            positionData.capsuleBottomPosition,
            positionData.centerPosition,
            sideDirection,
            rotate_x);

        positionData.rayTopPosition = Calculation::RotateLineSegment(
            positionData.capsuleBottomPosition,
            positionData.rayTopPosition,
            sideDirection,
            rotate_x);
    }

    //AABB
    mAABB.min = VGet(position.x - radius, position.y, position.z - radius);
    mAABB.max = VGet(position.x + radius, positionData.rayTopPosition.y, position.z + radius);

    DebugDrawer::Instance().InformationInput_string_VECTOR("faceDirection %f %f %f\n", faceDirection);
    DebugDrawer::Instance().InformationInput_AABB(mAABB.min, mAABB.max, GetColor(255, 0, 0));
}

void Player::Receive_CollisionResult()
{
    if (isCollisionCheck)
    {
        playerData.isGround = collision_result.isHitGround;
        position = collision_result.position_new;
        playerData.isPossibleWallRun = collision_result.isPossibleWallRun;
        if (VSize(collision_result.ishitWallNormal) != 0)
        {
            playerCalculation->SethitWallNormal(collision_result.ishitWallNormal);
        }
    }
    else
    {
        position = VAdd(position, velocity);
    }
}

void Player::DebugUpdate()
{
    //カプセル
    DebugDrawer::Instance().InformationInput_capsule(
        positionData.capsuleTopPosition,
        positionData.capsuleBottomPosition,
        radius,
        GetColor(255, 0, 0));

    //AABB
    VECTOR min = VGet(153.0f, 8.0f, 750.0f);
    VECTOR max = VGet(245.0f, 80.0f, 815.0f);
    //DebugDrawer::Instance().InformationInput_AABB(min, max, GetColor(255, 0, 0));
   

    //球体
    VECTOR nowFrame = MV1GetFramePosition(modelHandle, nowFrameNumber);
    DebugDrawer::Instance().InformationInput_sphere(nowFrame, 2.0f, GetColor(0, 0, 0));
    
    //string_VECTOR
    DebugDrawer::Instance().InformationInput_string_VECTOR("playerPosition.x %f\nplayerPosition.y %f\nplayerPosition.z %f\n", position);
    DebugDrawer::Instance().InformationInput_string_VECTOR("nowMoveDirection.x %f\nowMoveDirection.y %f\nowMoveDirection.z %f\n", nowMoveDirection);

    //string_int
    DebugDrawer::Instance().InformationInput_string_int("frame現在数%d\n", nowFrameNumber);

    //string_flaot
    DebugDrawer::Instance().InformationInput_string_float("coinCount %f\n", coinCount);
    DebugDrawer::Instance().InformationInput_string_float("JoyPad_x_left %f\n", -PadInput::GetJoyPad_x_left());
    DebugDrawer::Instance().InformationInput_string_float("JoyPad_y_left %f\n", -PadInput::GetJoyPad_y_left());
    DebugDrawer::Instance().InformationInput_string_float("degree_pad_now %f\n", degree_pad_now);
    DebugDrawer::Instance().InformationInput_string_float("playerDegree %f\n", GetDegree());

    //string_bool
    DebugDrawer::Instance().InformationInput_string_bool("isGround %d\n", playerData.isGround);
    DebugDrawer::Instance().InformationInput_string_bool("isIdle %d\n", playerData.isIdle);
    DebugDrawer::Instance().InformationInput_string_bool("isMove %d\n", playerData.isMove);
    DebugDrawer::Instance().InformationInput_string_bool("isRun %d\n", playerData.isRun);
    DebugDrawer::Instance().InformationInput_string_bool("isRunWall %d\n", playerData.isRunWall);
    DebugDrawer::Instance().InformationInput_string_bool("isUseWallJump %d\n", playerData.isUseWallJump);
    DebugDrawer::Instance().InformationInput_string_bool("isStopRun %d\n", playerData.isStopRun);
    DebugDrawer::Instance().InformationInput_string_bool("isJump %d\n", playerData.isJump);
    DebugDrawer::Instance().InformationInput_string_bool("isJumpFirst %d\n", playerData.isJumpFirst);
    DebugDrawer::Instance().InformationInput_string_bool("isJumpSecond %d\n", playerData.isJumpSecond);
    DebugDrawer::Instance().InformationInput_string_bool("isJumpAll %d\n", playerData.isJumpAll);
    DebugDrawer::Instance().InformationInput_string_bool("isWalljump %d\n", playerData.isWalljump);
    DebugDrawer::Instance().InformationInput_string_bool("isUseWallJump %d\n", playerData.isUseWallJump);
    DebugDrawer::Instance().InformationInput_string_bool("IsPushRT %d\n", playerData.IsPushRT);
    DebugDrawer::Instance().InformationInput_string_bool("isUseRoll %d\n", playerData.isUseRoll);
    DebugDrawer::Instance().InformationInput_string_bool("isFalling %d\n", playerData.isFalling);
    DebugDrawer::Instance().InformationInput_string_bool("isHanging %d\n", playerData.isHanging);
    DebugDrawer::Instance().InformationInput_string_bool("isHanging_now %d\n", playerData.isHanging_now);
    DebugDrawer::Instance().InformationInput_string_bool("isUseHanging %d\n", playerData.isUseHanging);
    DebugDrawer::Instance().InformationInput_string_bool("isPossibleWallRun %d\n", playerData.isPossibleWallRun);
    DebugDrawer::Instance().InformationInput_string_bool("isHangToCrouch %d\n", playerData.isHangToCrouch);

}