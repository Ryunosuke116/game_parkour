#include "Common.h"
#include <memory>
#include <vector>
#include "PlayerStateBase.h"
#include "PlayerData.h"
#include "PadInput.h"
#include "FallingIdle.h"
#include "AnimTime.h"
#include "Player.h"
#include "HitCheck.h"
#include "DebugDrawer.h"
#include "WorldSubSystem.h"
#include "CollisionObjectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
FallingIdle::FallingIdle(const int modelHandle) :
	PlayerStateBase(modelHandle)
{
    this->nowAnimState.PlayAnimSpeed = kPlayAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
FallingIdle::~FallingIdle()
{
	//MV1DetachAnim(modelHandle, nowAnimState.attachIndex);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> FallingIdle::Update(const VECTOR& cameraDirection,
    const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, 
    Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);
    PlayerData playerData = player.GetData();

    moveDirection = Command(cameraDirection, playerData, player);

    ResetIsJumps(playerData);

    VECTOR distancePlayerTopAndBottom = 
        VSub(player.GetPositionData().rayTopPosition,
            player.GetPositionData().rayBottomPosition);

    float playerSize = VSize(distancePlayerTopAndBottom);

    VECTOR spherePos = player.GetPositionData().rayTopPosition;

    //見直し
    //崖掴み判定
    if (playerData.isUseHanging)
    {
        //崖つかみ判定
        auto resultCheckCliff = HitCheck::CliffGrabbing(
            fieldObjects,
            player.GetPosition(),
            spherePos,
            player.GetFaceDirection(),
            kCliffRadius);
        
        //掴むところが平行だった場合
        //崖掴み時の情報を保存
        if (resultCheckCliff.isHitHanging)
        {
            //胴体座標
            const int kChestBoneNumber = MV1SearchFrame(modelHandle,
                "mixamorig:Spine1");
            VECTOR centerPosition = MV1GetFramePosition(modelHandle, kChestBoneNumber);
       
            Calculation::NearestResult nearestResult =
                Calculation::SphereMeshOutsideTriangleLine(resultCheckCliff.hangingPoly, spherePos);

            DebugDrawer::GetInstance().InformationInputLine(nearestResult.startLinePos,
                nearestResult.endLinePos, 
                GetColor(255, 0, 0));
            
            playerData.isHanging = resultCheckCliff.isHitHanging;
            playerData.isMove = false;
            isChangeState = true;
            player.playerCalculation->SetNearestResult(nearestResult);
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

        playerData.isUseWallJump = true;
        playerData.isUseHanging = true;
        isChangeState = true;
    }

    return std::make_pair(moveDirection, playerData);
}

VECTOR FallingIdle::Command(const VECTOR& cameraDirection,
    PlayerData& playerData,
    Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);
    auto collisionObjectManager = WorldSubSystem::GetInstance().GetSubSystem<CollisionObjectManager>();

    //moveDirを取得する
    moveDirection = Move(cameraDirection, playerData);
    JumpMove(playerData, player);
    RollMove(playerData);
    WallRunMove(playerData, player, collisionObjectManager->GetCollisionObject("field"));

    if (playerData.isRunWall)
    {
        moveDirection = VGet(0.0f, 1.0f, 0.0f);
    }

    return moveDirection;
}

void FallingIdle::Exit(PlayerData& playerData)
{
    playerData.isFalling = false;
}