#include "common.h"
#include <memory>
#include <vector>
#include "PlayerStateActionBase.h"
#include "PlayerData.h"
#include "PadInput.h"
#include "Falling_Idle.h"
#include "AnimTime.h"
#include "Player.h"
#include "HitCheck.h"
#include "DebugDrawer.h"

/// <summary>
/// コンストラクタ
/// </summary>
Falling_Idle::Falling_Idle(int& modelHandle,
	AnimState& oldAnimState, AnimState& nowAnimState) :
	PlayerStateActionBase(modelHandle, oldAnimState,nowAnimState)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Falling_Idle::~Falling_Idle()
{
	//MV1DetachAnim(modelHandle, nowAnimState.AttachIndex);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="modelHandle"></param>
void Falling_Idle::Initialize(int& modelHandle, Player& player)
{
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;

    // ３Ｄモデルの０番目のアニメーションをアタッチする
    this->nowAnimState.AttachIndex = MV1AttachAnim(this->modelHandle, animNum::falling_Idle);

    this->nowAnimState.PlayTime_anim = 0.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// 更新
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="collisionObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> Falling_Idle::Update(const VECTOR& cameraDirection,
    const std::vector<std::shared_ptr<BaseObject>>& collisionObjects, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    PlayerData playerData = player.GetData();

    moveDirection = Command(cameraDirection, playerData, player);

    FlagReset_jump(playerData);

    VECTOR head = player.GetPositionData().position_top_ray;

    //見直し
    //崖掴み判定
    if (playerData.isUse_Hanging)
    {
        auto result_cliff = HitCheck::CliffGrabbing(collisionObjects,
            head, player.GetMoveDirection_now(), cliff_radius);
        
        //掴むところが平行だった場合
        //崖掴み時の情報を保存
        if (result_cliff.isHitHanging)
        {
            //胴体座標
            VECTOR centerPosition = MV1GetFramePosition(modelHandle, 2);
       
            Calculation::NearestResult nearestResult = Calculation::SphereMeshOutsideTriangle_line(result_cliff.hangingPoly, head);
            DebugDrawer::Instance().InformationInput_line(nearestResult.linePos_start, nearestResult.linePos_end, GetColor(255, 0, 0));
       
            float difference_y = nearestResult.linePos_start.y - nearestResult.linePos_end.y;
            float abs_value = abs(difference_y);
            
                playerData.isHanging = result_cliff.isHitHanging;
                isChangeState = true;
                player.playerCalculation->SetNearestResult(nearestResult);
            if (abs_value <= 10.0f)
            {
            }
        }
    }


    //着地したときに動いているかどうかで次のアニメーションを決める
    if (playerData.isGround)
    {
        if (playerData.isMove)
        {
            playerData.isRun = true;
        }
        else
        {
            playerData.isIdle = true;
        }

        playerData.isUse_wallJump = true;
        playerData.isUse_Hanging = true;
        isChangeState = true;
    }

    return std::make_pair(moveDirection, playerData);
}

VECTOR Falling_Idle::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //moveDirを取得する
    moveDirection = Move(cameraDirection, playerData);
    JumpMove(playerData, player);
    RollMove(playerData);
    WallRunMove(playerData, player);

    if (playerData.isRun_wall)
    {
        moveDirection = VGet(0.0f, 1.0f, 0.0f);
    }

    return moveDirection;
}

void Falling_Idle::Enter(PlayerData& playerData)
{
    playerData.isFalling = true;
}

void Falling_Idle::Exit(PlayerData& playerData)
{
    playerData.isFalling = false;
}