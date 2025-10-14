#include "Common.h"
#include <fstream>
#include"PlayerState.h"
#include "AnimTime.h"
#include "PadInput.h"
#include "EffectManager.h"
#include "Player.h"
#include "HitCheck.h"
#include "Calculation.h"
#include "DebugDrawer.h"
#include "Camera.h"
#include "JsonManager.h"
#include "WorldSubSystem.h"
#include "CollisionObjectManager.h"
#include "GameInstanceSubSystem.h"

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player() :
    BaseChara(),
    startWalkTime(-1),
    nowMoveDirection(VGet(0.0f, 0.0f, 0.0f)),
    normalVelocity(VGet(0.0f,0.0f,0.0f)),
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
    MV1DeleteModel(modelHandle);
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

void Player::Create()
{
    const std::string jsonName = "player";
    Load(JsonManager::GetInstance().GetJsons(jsonName));
    playerCalculation = std::make_shared<PlayerCalculation>();
    animationChanger = std::make_shared<AnimationChanger>();
    animationChanger->Create(modelHandle);
    MV1SetScale(modelHandle, VGet(kModelScale, kModelScale, kModelScale));
}

/// <summary>
/// 初期化
/// </summary>
void Player::Initialize()
{
    const VECTOR initPosition = VGet(3.0f, 9.0f, -1210.0f);

    position = initPosition;
    targetMoveDirection = VGet(0.0f, 0.0f, 0.0f);
    radian = 0.0f;
    rotateX = 0.0f;
    startWalkTime = 0.0f;

    MV1SetPosition(modelHandle, position);

    isPush = false;

    playerData.isIdle = false;
    playerData.isGround = true;
    playerData.isJump = false;
    playerData.isSecondJump = false;
    playerData.isAllJump = false;
    playerData.isWalljump = false;
    playerData.isMove = false;
    playerData.isWalk = true;
    playerData.isRoll = false;
    playerData.isSprint = false;
    playerData.isStopRun = false;
    playerData.isUseRoll = false;
    playerData.isJumpPlayAnim = false;
    playerData.isFalling = false;
    playerData.isHanging = false;
    playerData.isNowHanging = false;
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
    effectTimer = 0.0f;
    nowMoveDirection = VGet(0.0f, 0.0f, 0.0f);

    MV1SetRotationXYZ(modelHandle, VGet((rotateX * DX_PI_F / 180.0f), radian + DX_PI_F, 0.0f));

    animationChanger->Initialize(animNum::walk, modelHandle, nowState, playerData, *this);
}

/// <summary>
/// 更新
/// </summary>
void Player::Update()
{
    //床抜けバグ対策
    CounterplanBug();

     //positionData更新
    CollisionUpdate();

    //リセット
    moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //接地中であればロールアクションを使えるように
    if (playerData.isGround && !playerData.isRoll)
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

    //キャラの進行方向、向く方向ベクトル調整
    MoveDirectionUpdate();

    //キャラクターの向く方向を更新
    UpdateAngle(faceDirection);
   
    //状態変更
    ChangeState();

    nowState->MotionUpdate(playerData);

    //move計算
    velocity = playerCalculation->Update(
        nowMoveDirection,
        nowState->GetNowAnimState().playAnimTime,
        animationChanger->NowGetAnimNumber(),
        playerData);

    EffectUpdate();

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

    DebugUpdate();
    
    nowState->Draw();
}

/// <summary>
/// ゲームシーンのスタート時の更新処理
/// </summary>
/// <param name="timer"></param>
void Player::StartUpdate(const float timer)
{
    const float kMaxTimer = 40.0f;

    velocity = VGet(0.0f, 0.0f, 0.5f);

    if (timer <= kMaxTimer)
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
void Player::FinishUpdate(const float timer)
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
    const std::string jsonName = "player";
    Load(JsonManager::GetInstance().GetJsons(jsonName));
    playerCalculation = std::make_shared<PlayerCalculation>();
    animationChanger = std::make_shared<AnimationChanger>();
    MV1SetScale(modelHandle, VGet(kModelScale, kModelScale, kModelScale));
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void Player::ResultInitialize()
{
    const VECTOR kInitPosition = VGet(0.0f, 9.0f, -1206.0f);

    position = kInitPosition;
    radian = 0.0f;

    MV1SetPosition(modelHandle, position);

    MV1SetRotationXYZ(modelHandle, VGet((rotateX * DX_PI_F / 180.0f), radian + DX_PI_F, 0.0f));

    animationChanger->ResultInitialize(animNum::victory, modelHandle, nowState, playerData, *this);
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
    const bool isNotAction = !playerData.isHanging &&
        !playerData.isHangToCrouch &&
        !playerData.isFalling &&
        !playerData.isJump &&
        !playerData.isRoll;

    const bool isAction = !playerData.isHanging &&
        !playerData.isHangToCrouch &&
        !playerData.isWalljump;

    isCalcMoveVec = VSize(moveDirection) != 0;

    //移動方向ベクトルが0でない場合代入
    if (isCalcMoveVec)
    {
        targetMoveDirection = moveDirection;
    }

    //通常時は進行方向にすぐ向くように
    if (isNotAction)
    {
        const float kLeapSpeed = 0.15f;

        nowMoveDirection = Calculation::Leap(
            nowMoveDirection,
            targetMoveDirection,
            kLeapSpeed);

        nowMoveDirection.y = 0.0f;
    }
    //特定のアクション時は移動方向を変えられないように
    else if (isAction)
    {
        const float kSpeed = 0.03f;

        //ゆっくり最新の方向に向く
        nowMoveDirection = Calculation::Leap(
            nowMoveDirection,
            targetMoveDirection, 
            kSpeed);

        nowMoveDirection.y = 0.0f;
    }

    if (!playerData.isRunWall)
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

        nowState = animationChanger->ChangeState(modelHandle, *this, playerData, nowState);
    }
}

/// <summary>
/// コリジョンデータの更新
/// </summary>
void Player::CollisionUpdate()
{
    bool isReverse = false;

    const int left = MV1SearchFrame(modelHandle, "mixamorig:LeftHandIndex4");
    const int right = MV1SearchFrame(modelHandle, "mixamorig:RightHandMiddle4_end");
    const int head = 7;
    const float kRightAngle = 90.0f;
    const VECTOR kVerticalShaft = VGet(0.0f, 1.0f, 0.0f);
    const VECTOR kRotatePosition = VAdd(position, faceDirection);
    const VECTOR kAddCenterPosition = VGet(0.0f, 10.0f, 0.0f);       //キャラの足もとから中心までの差

    playerCalculation->SetLeftHandPos(MV1GetFramePosition(modelHandle, left));
    playerCalculation->SetRightHandPos(MV1GetFramePosition(modelHandle, right));
    positionData.oldPosition = position;

    //床衝突判定用ray
    positionData.centerPosition = VAdd(position, kAddCenterPosition);
    positionData.rayTopPosition = VGet(position.x, positionData.centerPosition.y + height, position.z);
    positionData.rayBottomPosition = position;
    
    positionData.rayTopPosition.x = positionData.rayBottomPosition.x;
    positionData.rayTopPosition.z = positionData.rayBottomPosition.z;
     
    //投影で歩くためのごまかし
    if ((playerData.isRun &&
        !playerData.isJump) ||
        playerData.isRoll)
    {
        positionData.rayBottomPosition.y -= playerCalculation->GetNowMoveSpeed();
    }

    //キャラの横軸を求める
    positionData.sideShaft = Calculation::RotateLineSegment(
        position,
        kRotatePosition,
        kVerticalShaft,
        kRightAngle);

    //壁衝突判定用カプセル
    positionData.capsuleBottomPosition = position;

    positionData.capsuleTopPosition = VGet(
        positionData.capsuleBottomPosition.x,
        positionData.centerPosition.y + height,
        positionData.capsuleBottomPosition.z);

    
    //調整
    //壁衝突判定用カプセル
    //カプセルなので上下、設定座標よりも半径分足し引きする
    positionData.capsuleBottomPosition.y += radius;
    positionData.capsuleTopPosition.y -= radius;

    //bottomPosよりも下にいかないように
    //capsule
    if (playerData.isRoll)
    {
        int foot = MV1SearchFrame(modelHandle, "mixamorig:RightToeBase");
        int hand = MV1SearchFrame(modelHandle, "mixamorig:RightHandIndex3");
        VECTOR handPos = MV1GetFramePosition(modelHandle, hand);
        VECTOR footPos = MV1GetFramePosition(modelHandle, foot);
        //高さが低い方をカプセルの下部にする
        if (handPos.y > footPos.y)
        {
            positionData.capsuleBottomPosition.y = footPos.y + radius;
        }
        else
        {
            positionData.capsuleBottomPosition.y = handPos.y + radius;
        }
    }

    const VECTOR sideDirection = VNorm(VSub(position, positionData.sideShaft));
    
    //縦の軸の線分を回転させる
    if (playerData.isRunWall)
    {
        positionData.capsuleTopPosition = Calculation::RotateLineSegment(
            positionData.capsuleBottomPosition,
            positionData.capsuleTopPosition,
            sideDirection,
            rotateX);

        positionData.centerPosition = Calculation::RotateLineSegment(
            positionData.capsuleBottomPosition,
            positionData.centerPosition,
            sideDirection,
            rotateX);

        positionData.rayTopPosition = Calculation::RotateLineSegment(
            positionData.capsuleBottomPosition,
            positionData.rayTopPosition,
            sideDirection,
            rotateX);
    }

    //AABB
    mAABB.min = VGet(position.x - radius, position.y, position.z - radius);
    mAABB.max = VGet(position.x + radius, positionData.rayTopPosition.y, position.z + radius);
}

void Player::Receive_CollisionResult()
{
    if (isCollisionCheck)
    {
        playerCalculation->SetNowGroundRayPoly(resultCollision.nowGroundRayPoly);
        playerData.isGround = resultCollision.nowGroundRayPoly.HitFlag;
        position = resultCollision.newPosition;
    }
    else
    {
        position = VAdd(position, velocity);
    }
}

void Player::EffectUpdate()
{
    if (playerData.isRun)
    {
        //エフェクトマネージャーのポインタを参照
        std::shared_ptr<EffectManager> effectManager = GameInstanceSubSystem::GetInstance().GetSubSystem<EffectManager>().lock();
        const float kMaxEffectTimer = 10.0f;
        const float kAddEffectPositionY = 2.0f;
        const VECTOR kEffectScale = VGet(4.0f, 4.0f, 4.0f);

        effectTimer++;

        if (effectTimer >= kMaxEffectTimer)
        {
            VECTOR effectPosition = position;

            effectPosition.y += kAddEffectPositionY;
            effectManager->PlayEffect("foot_smoke");
            effectManager->SetScale(kEffectScale, "foot_smoke");
            effectManager->SetPosition(effectPosition, "foot_smoke");
            effectTimer = 0.0f;
        }
    }
}

void Player::DebugUpdate()
{
    float padRadian = atan2f(-PadInput::GetJoyPadXLeft(), -PadInput::GetJoyPadYLeft());

    //度数計算
    float nowPadDegree = Calculation::RadToDeg(padRadian);

    //カプセル
    DebugDrawer::GetInstance().InformationInput_capsule(positionData.capsuleTopPosition,
        positionData.capsuleBottomPosition, radius, GetColor(255, 0, 0));
    
    //string_VECTOR
    DebugDrawer::GetInstance().InformationInput_string_VECTOR("playerPosition.x %f\nplayerPosition.y %f\nplayerPosition.z %f\n", position);
    DebugDrawer::GetInstance().InformationInput_string_VECTOR("nowMoveDirection.x %f\nowMoveDirection.y %f\nowMoveDirection.z %f\n", nowMoveDirection);
    DebugDrawer::GetInstance().InformationInput_string_VECTOR("faceDirection %f %f %f\n", faceDirection);

    //string_int
    DebugDrawer::GetInstance().InformationInput_string_int("frame現在数%d\n", nowFrameNumber);
    DebugDrawer::GetInstance().InformationInput_string_int("coinCount %f\n", coinCount);

    //string_flaot
    DebugDrawer::GetInstance().InformationInput_string_float("JoyPad_x_left %f\n", -PadInput::GetJoyPadXLeft());
    DebugDrawer::GetInstance().InformationInput_string_float("JoyPad_y_left %f\n", -PadInput::GetJoyPadYLeft());
    DebugDrawer::GetInstance().InformationInput_string_float("nowPadDegree %f\n", nowPadDegree);
    DebugDrawer::GetInstance().InformationInput_string_float("playerDegree %f\n", GetDegree());

    //string_bool
    DebugDrawer::GetInstance().InformationInput_string_bool("isGround %d\n", playerData.isGround);
    DebugDrawer::GetInstance().InformationInput_string_bool("isIdle %d\n", playerData.isIdle);
    DebugDrawer::GetInstance().InformationInput_string_bool("isMove %d\n", playerData.isMove);
    DebugDrawer::GetInstance().InformationInput_string_bool("isRun %d\n", playerData.isRun);
    DebugDrawer::GetInstance().InformationInput_string_bool("isRunWall %d\n", playerData.isRunWall);
    DebugDrawer::GetInstance().InformationInput_string_bool("isUseWallJump %d\n", playerData.isUseWallJump);
    DebugDrawer::GetInstance().InformationInput_string_bool("isStopRun %d\n", playerData.isStopRun);
    DebugDrawer::GetInstance().InformationInput_string_bool("isJump %d\n", playerData.isJump);
    DebugDrawer::GetInstance().InformationInput_string_bool("isFirstJump %d\n", playerData.isFirstJump);
    DebugDrawer::GetInstance().InformationInput_string_bool("isSecondJump %d\n", playerData.isSecondJump);
    DebugDrawer::GetInstance().InformationInput_string_bool("isAllJump %d\n", playerData.isAllJump);
    DebugDrawer::GetInstance().InformationInput_string_bool("isWalljump %d\n", playerData.isWalljump);
    DebugDrawer::GetInstance().InformationInput_string_bool("isUseWallJump %d\n", playerData.isUseWallJump);
    DebugDrawer::GetInstance().InformationInput_string_bool("isRoll %d\n", playerData.isRoll);
    DebugDrawer::GetInstance().InformationInput_string_bool("isUseRoll %d\n", playerData.isUseRoll);
    DebugDrawer::GetInstance().InformationInput_string_bool("isFalling %d\n", playerData.isFalling);
    DebugDrawer::GetInstance().InformationInput_string_bool("isHanging %d\n", playerData.isHanging);
    DebugDrawer::GetInstance().InformationInput_string_bool("isNowHanging %d\n", playerData.isNowHanging);
    DebugDrawer::GetInstance().InformationInput_string_bool("isUseHanging %d\n", playerData.isUseHanging);
    DebugDrawer::GetInstance().InformationInput_string_bool("isHangToCrouch %d\n", playerData.isHangToCrouch);
}

/// @brief プレイヤーが特定の位置より下に落下した場合に、位置と状態をリセット
void Player::CounterplanBug()
{
    const float kMaxPosY = -10.0f;
    const VECTOR kResetPosition = VGet(3.0f, 9.0f, -1200.0f);

    if (position.y < kMaxPosY)
    {
        position = kResetPosition;
        playerData.isIdle = true;
        playerData.isGround = false;
        playerData.isJump = false;
        playerData.isSecondJump = false;
        playerData.isAllJump = false;
        playerData.isWalljump = false;
        playerData.isMove = false;
        playerData.isWalk = false;
        playerData.isRoll = false;
        playerData.isSprint = false;
        playerData.isStopRun = false;
        playerData.isUseRoll = false;
        playerData.isJumpPlayAnim = false;
        playerData.isFalling = false;
        playerData.isHanging = false;
        playerData.isNowHanging = false;
        playerData.isUseHanging = true;
        playerData.isHangToCrouch = false;
        playerData.isRun = false;
        playerData.isRunWall = false;
        playerData.isUseWallJump = true;
        playerData.isDash = false;
        playerData.isWallClimb = false;
    }
}