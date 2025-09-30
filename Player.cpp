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
#include "GameInstanceSubSystem.h"

/// <summary>
/// コンストラクタ
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
    MV1SetScale(modelHandle, VGet(modelScale, modelScale, modelScale));
}

/// <summary>
/// 初期化
/// </summary>
void Player::Initialize()
{
    const VECTOR initPosition = VGet(3.02443838f, 9.00285912f, -1215.93481f);

    const VECTOR pos = VGet(-1663.0f, 688.5f, 788.0f);

    position = initPosition;
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
    //床抜けバグ対策
    CounterplanBug();

     //positionData更新
    CollisionUpdate();

    //エフェクトマネージャーのポインタを参照
    std::shared_ptr<EffectManager> effectManager = GameInstanceSubSystem::GetInstance().GetSubSystem<EffectManager>().lock();

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
        animationChanger->GetAnimNumber_now(),
        playerData);

    if (playerData.isRun)
    {
        effectTimer++;
        if (effectTimer >= 10.0f)
        {
            const VECTOR kScale = VGet(6.0f, 6.0f, 6.0f);
            VECTOR effectPosition = position;

            effectPosition.y += 2.0f;
            effectManager->PlayEffect("foot_smoke");
            effectManager->SetScale(kScale, "foot_smoke");
            effectManager->SetPosition(effectPosition,"foot_smoke");
            effectTimer = 0.0f;
        }
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

    float radian_pad = atan2f(-PadInput::GetJoyPadXLeft(), -PadInput::GetJoyPadYLeft());

    //度数計算
    degree_pad_now = Calculation::RadToDeg(radian_pad);

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
    const std::string jsonName = "player";
    Load(JsonManager::GetInstance().GetJsons(jsonName));
    playerCalculation = std::make_shared<PlayerCalculation>();
    animationChanger = std::make_shared<AnimationChanger>();
    MV1SetScale(modelHandle, VGet(modelScale, modelScale, modelScale));
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
    const bool isAction = !playerData.isHanging &&
        !playerData.isHangToCrouch &&
        !playerData.isFalling &&
        !playerData.isJump &&
        !playerData.isRoll;

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
    //positionData更新
    
    const int left = MV1SearchFrame(modelHandle, "mixamorig:LeftHandIndex4");
    const int right = MV1SearchFrame(modelHandle, "mixamorig:RightHandMiddle4_end");
    const int head = 7;
    const float kRightAngle = 90.0f;
    const VECTOR kVerticalShaft = VGet(0.0f, 1.0f, 0.0f);
    const VECTOR kRotatePosition = VAdd(position, faceDirection);
    const VECTOR kAddCenterPosition = VGet(0.0f, 10.0f, 0.0f);       //キャラの足もとから中心までの差

    playerCalculation->SetHandPos_left(MV1GetFramePosition(modelHandle, left));
    playerCalculation->SetHandPos_right(MV1GetFramePosition(modelHandle, right));
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
        positionData.rayBottomPosition.y -= playerCalculation->GetMoveSpeed_now();
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

    const VECTOR kSideDirection = VNorm(VSub(position, positionData.sideShaft));
    
    //縦の軸の線分を回転させる
    if (playerData.isRunWall)
    {
        positionData.capsuleTopPosition = Calculation::RotateLineSegment(
            positionData.capsuleBottomPosition,
            positionData.capsuleTopPosition,
            kSideDirection,
            rotate_x);

        positionData.centerPosition = Calculation::RotateLineSegment(
            positionData.capsuleBottomPosition,
            positionData.centerPosition,
            kSideDirection,
            rotate_x);

        positionData.rayTopPosition = Calculation::RotateLineSegment(
            positionData.capsuleBottomPosition,
            positionData.rayTopPosition,
            kSideDirection,
            rotate_x);
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

void Player::DebugUpdate()
{
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
    DebugDrawer::GetInstance().InformationInput_string_float("degree_pad_now %f\n", degree_pad_now);
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

void Player::CounterplanBug()
{
    if (position.y < -10.0f)
    {
        position = VGet(3.02443838f, 9.00285912f, -1215.93481f);
        isChange_falling = false;
        playerData.isIdle = true;
        playerData.isGround = false;
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
    }
}