#include "Common.h"
#include <memory>
#include <vector>
#include "PadInput.h"
#include "PlayerStateBase.h"
#include "BracedHangToCrouch.h"
#include "AnimTime.h"
#include "Player.h"
#include "HitCheck.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldAnimState"></param>
/// <param name="nowAnimState"></param>
/// <param name="playerData"></param>
BracedHangToCrouch::BracedHangToCrouch(const int modelHandle) :
    PlayerStateBase(modelHandle)
{
    this->nowAnimState.PlayAnimSpeed = kPlayAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
BracedHangToCrouch::~BracedHangToCrouch()
{

}

/// <summary>
/// 更新
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> BracedHangToCrouch::Update(
    const VECTOR& cameraDirection,
    const std::vector<std::weak_ptr<BaseObject>>& fieldObjects,
    Player& player)
{
    VECTOR moveDirection = player.GetNowMoveDirection();

    PlayerData playerData = player.GetData();

    //上り終わったらplayerDataを初期化
    if (nowAnimState.playAnimTime >=
        nowAnimState.totalPlayAnimTime - 1.0f)
    {
        isChangeState = true;
        player.SetIsCollisionCheck(true);
        if (!playerData.isMove)
        {
            playerData.isIdle = true;
        }
        else
        {
            playerData.isRun = true;
        }
        playerData.isGround = player.playerCalculation->GetIsWhenClimbingHitGround();
    }

    return std::make_pair(moveDirection, playerData);
}

VECTOR BracedHangToCrouch::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //moveDirを取得する
    moveDirection = Move(cameraDirection, playerData);

    return moveDirection;
}

void BracedHangToCrouch::Exit(PlayerData& playerData)
{
    playerData.isHangToCrouch = false;
}
