#include "common.h"
#include <memory>
#include <vector>
#include "PlayerStateBase.h"
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
	PlayerStateBase(modelHandle, oldAnimState, nowAnimState)
{
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
Falling_Idle::~Falling_Idle()
{
	//MV1DetachAnim(modelHandle, nowAnimState.AttachIndex);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> Falling_Idle::Update(const VECTOR& cameraDirection,
    const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    PlayerData playerData = player.GetData();

    moveDirection = Command(cameraDirection, playerData, player);

    FlagReset_jump(playerData);

    VECTOR head = player.GetPositionData().rayTopPosition;

    //見直し
    //崖掴み判定
    if (playerData.isUse_Hanging)
    {
        //崖つかみ判定
        auto result_cliff = HitCheck::CliffGrabbing(
            fieldObjects,
            player.GetPosition(),
            head,
            player.GetNowMoveDirection(),
            cliff_radius);
        
        //掴むところが平行だった場合
        //崖掴み時の情報を保存
        if (result_cliff.isHitHanging)
        {
            //胴体座標
            VECTOR centerPosition = MV1GetFramePosition(modelHandle, 2);
       
            Calculation::NearestResult nearestResult =
                Calculation::SphereMeshOutsideTriangle_line(result_cliff.hangingPoly, head);

            DebugDrawer::Instance().InformationInput_line(nearestResult.linePos_start, nearestResult.linePos_end, GetColor(255, 0, 0));
       
            float difference_y = nearestResult.linePos_start.y - nearestResult.linePos_end.y;
            float abs_value = abs(difference_y);
            
            if (abs_value <= 1e-2)
            {
                playerData.isHanging = result_cliff.isHitHanging;
                isChangeState = true;
                player.playerCalculation->SetNearestResult(nearestResult);
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

    if (playerData.isRunWall)
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