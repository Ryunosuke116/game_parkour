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



/// <summary>
/// /インストラクタ
/// </summary>
Player::Player() :
    BaseChara(),
    centerPosition(VGet(0.0f, 0.0f, 0.0f)),
    moveDirection_now(VGet(0.0f, 0.0f, 0.0f)),
    isCalc_moveVec(false),
    playerData({false})
{
   
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{

}

/// <summary>
/// 読み込み
/// </summary>
/// <param name="jsonData"></param>
void Player::Load(const nlohmann::json& jsonData)
{
    std::string path = jsonData["playerPath"];

    modelHandle = MV1LoadModel(path.c_str());
    MV1SetScale(modelHandle, VGet(modelScale, modelScale, modelScale));
    playerCalculation = std::make_shared<PlayerCalculation>();
    animationChanger = std::make_shared<AnimationChanger>();
}

/// <summary>
/// 初期化
/// </summary>
void Player::Initialize()
{
    position = VGet(0.0f, 6.74f, 0.0f);

    MV1SetRotationXYZ(modelHandle, VGet(0, 0, 0));

    isPush = false;
    isChange_falling = false;

    playerData.isIdle = true;
    playerData.isGround = true;
    playerData.isJump = false;
    playerData.isJump_second = false;
    playerData.isMove = false;
    playerData.isRoll = false;
    playerData.isSprint = false;
    playerData.isStopRun = false;
    playerData.isJumpAll = false;
    playerData.isUse_Roll = false;
    playerData.isJump_PlayAnim = false;
    playerData.isFalling = false;
    playerData.isHanging = false;
    playerData.isHanging_now = false;
    playerData.isUse_Hanging = true;
    playerData.isHang_to_Crouch = false;
    playerData.isRun = false;
    playerData.isRun_wall = false;
    playerData.isUse_wallJump = true;
    isCalc = false;
    isCalc_moveVec = false;
    isCollisionCheck = true;
   
    coinCount = 0;
    degree_pad_now = 0.0f;
    effectTimer = 0.0f;
    padInput_now = VGet(0.0f, 0.0f, 0.0f);
    moveDirection_now = VGet(0.0f, 0.0f, 1.0f);

    animationChanger->Initialize(-1, modelHandle, nowState, playerData, *this);

}

/// <summary>
/// 更新
/// </summary>
void Player::Update(const VECTOR& cameraDirection, 
    std::shared_ptr<EffectManager>& effectManager,
    const std::vector<std::shared_ptr<BaseObject>>& fieldObjects)
{
     //positionData更新
    CollisionUpdate();

    //リセット
    moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //接地中であればロールアクションを使えるように
    if (playerData.isGround && !playerData.isRoll)
    {
        playerData.isUse_Roll = false;
    }

    //stateに応じた挙動処理
    auto [moveDirection_new, data_new] = nowState->Update(cameraDirection, fieldObjects, *this);

    if (playerData.isHang_to_Crouch)
    {
        position = VAdd(position, moveDirection_new);
    }
    else
    {
        moveDirection = moveDirection_new;
    }

    playerData = data_new;

    bool isFree = !playerData.isHanging
        && !playerData.isHang_to_Crouch &&
        !playerData.isFalling && 
        !playerData.isJump && 
        !playerData.isRoll;

    isCalc_moveVec = VSize(moveDirection) != 0;

    //移動方向ベクトルが0でない場合コピー
    if (isCalc_moveVec)
    {
        targetMoveDirection = moveDirection;
    }

    //通常時は進行方向にすぐ向くように
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

    UpdateAngle(moveDirection_now);
   
    //状態変更
    ChangeState();

    isChange_falling = nowState->MotionUpdate(playerData);


    //move計算
    if (!playerData.isHang_to_Crouch)
    {
        moveVec = playerCalculation->Update(moveDirection_now,
            nowState->GetNowAnimState().PlayTime_anim,
            animationChanger->GetAnimNumber_now(), playerData);
    }

    if (!playerData.isRun)
    {
        positionData.position_bottom_ray = position;
        positionData.position_bottom_ray.y -= 0.1f;
    }

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
    
    playerCalculation->SetHandPos_left(MV1GetFramePosition(modelHandle, 65));
    playerCalculation->SetHandPos_right(MV1GetFramePosition(modelHandle, 106));
    centerPosition = MV1GetFramePosition(modelHandle, 7);

    //ray
    VECTOR position_center = VScale(VAdd(MV1GetFramePosition(modelHandle, 7), position), 0.5f);
    position_center = VGet(position.x, position_center.y, position.z);
    positionData.position_top_ray = VGet(position.x, position_center.y + height, position.z);
    positionData.position_bottom_ray = position;
    
    positionData.position_top_ray.x = positionData.position_bottom_ray.x;
    positionData.position_top_ray.z = positionData.position_bottom_ray.z;
    positionData.position_bottom_ray.y -= 0.1f;
     
    //投影で歩くためのごまかし
    if (playerData.isRun && !playerData.isJump)
    {
        positionData.position_bottom_ray.y -= playerCalculation->GetMoveSpeed_now();
    }

    //カプセル
    positionData.position_top_Capsule = VGet(position_center.x, position_center.y + height, centerPosition.z);
    positionData.position_bottom_Capsule = position;
    
    //調整
    //カプセル
    positionData.position_top_Capsule.x = positionData.position_bottom_Capsule.x;
    positionData.position_top_Capsule.z = positionData.position_bottom_Capsule.z;
    positionData.position_bottom_Capsule.y += radius;
    positionData.position_top_Capsule.y -= radius;

    //bottomPosよりも下にいかないように
    //capsule
    if (positionData.position_top_Capsule.y <= positionData.position_bottom_Capsule.y)
    {
        positionData.position_top_Capsule.y = positionData.position_bottom_Capsule.y;
    }

}

void Player::Receive_CollisionResult()
{
    if (isCollisionCheck)
    {
        playerData.isGround = collision_result.isHitGround;
        position = collision_result.position_new;
        playerData.isPossible_wallRun = collision_result.isPossible_wallRun;
        if (VSize(collision_result.isHitWall_normal) != 0)
        {
            playerCalculation->SetHitWall_normal(collision_result.isHitWall_normal);
        }
    }
}

void Player::DebugUpdate()
{
    //カプセル
    DebugDrawer::Instance().InformationInput_capsule(positionData.position_top_Capsule,
        positionData.position_bottom_Capsule, radius,
        GetColor(255, 0, 0));


    //AABB
    VECTOR min = VGet(153.0f, 8.0f, 750.0f);
    VECTOR max = VGet(245.0f, 80.0f, 815.0f);
    DebugDrawer::Instance().InformationInput_AABB(min, max, GetColor(255, 0, 0));
   

    //球体
    VECTOR nowFrame = MV1GetFramePosition(modelHandle, nowFrameNumber);
    DebugDrawer::Instance().InformationInput_sphere(nowFrame, 2.0f, GetColor(0, 0, 0));
    
    //string_VECTOR
    DebugDrawer::Instance().InformationInput_string_VECTOR("playerPosition.x %f\nplayerPosition.y %f\nplayerPosition.z %f\n", position);
    
    //string_flaot
    DebugDrawer::Instance().InformationInput_string_float("frame現在数%d\n", nowFrameNumber);
    DebugDrawer::Instance().InformationInput_string_float("coinCount %f\n", angle);
    DebugDrawer::Instance().InformationInput_string_float("JoyPad_x_left %f\n", -PadInput::GetJoyPad_x_left());
    DebugDrawer::Instance().InformationInput_string_float("JoyPad_y_left %f\n", -PadInput::GetJoyPad_y_left());
    DebugDrawer::Instance().InformationInput_string_float("degree_pad_now %f\n", degree_pad_now);

    //string_bool
    DebugDrawer::Instance().InformationInput_string_bool("isGround %d\n", playerData.isGround);
    DebugDrawer::Instance().InformationInput_string_bool("isIdle %d\n", playerData.isIdle);
    DebugDrawer::Instance().InformationInput_string_bool("isMove %d\n", playerData.isMove);
    DebugDrawer::Instance().InformationInput_string_bool("isRun %d\n", playerData.isRun);
    DebugDrawer::Instance().InformationInput_string_bool("isRun_wall %d\n", playerData.isRun_wall);
    DebugDrawer::Instance().InformationInput_string_bool("isUse_wallJump %d\n", playerData.isUse_wallJump);
    DebugDrawer::Instance().InformationInput_string_bool("isStopRun %d\n", playerData.isStopRun);
    DebugDrawer::Instance().InformationInput_string_bool("isJump %d\n", playerData.isJump);
    DebugDrawer::Instance().InformationInput_string_bool("isJump_first %d\n", playerData.isJump_first);
    DebugDrawer::Instance().InformationInput_string_bool("isJump_second %d\n", playerData.isJump_second);
    DebugDrawer::Instance().InformationInput_string_bool("isJumpAll %d\n", playerData.isJumpAll);
    DebugDrawer::Instance().InformationInput_string_bool("isUse_wallJump %d\n", playerData.isUse_wallJump);
    DebugDrawer::Instance().InformationInput_string_bool("isRoll %d\n", playerData.isRoll);
    DebugDrawer::Instance().InformationInput_string_bool("isUse_roll %d\n", playerData.isUse_Roll);
    DebugDrawer::Instance().InformationInput_string_bool("isFalling %d\n", playerData.isFalling);
    DebugDrawer::Instance().InformationInput_string_bool("isHanging %d\n", playerData.isHanging);
    DebugDrawer::Instance().InformationInput_string_bool("isHanging_now %d\n", playerData.isHanging_now);
    DebugDrawer::Instance().InformationInput_string_bool("isUse_Hanging %d\n", playerData.isUse_Hanging);
    DebugDrawer::Instance().InformationInput_string_bool("isPossible_wallRun %d\n", playerData.isPossible_wallRun);
    DebugDrawer::Instance().InformationInput_string_bool("isHang_to_Crouch %d\n", playerData.isHang_to_Crouch);

}


void Player::Update() {};