#include "common.h"
#include <memory>
#include <vector>
#include "PadInput.h"
#include "PlayerData.h"
#include "PlayerStateActionBase.h"
#include "Hanging_Idle.h"
#include "AnimTime.h"
#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldAnimState"></param>
/// <param name="nowAnimState"></param>
/// <param name="playerData"></param>
Hanging_Idle::Hanging_Idle(int& modelHandle,
    AnimState& oldAnimState, AnimState& nowAnimState) :
    PlayerStateActionBase(modelHandle, oldAnimState, nowAnimState)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Hanging_Idle::~Hanging_Idle()
{

}

/// <summary>
/// 初期化
/// </summary>
/// <param name="modelHandle"></param>
void Hanging_Idle::Initialize(int& modelHandle, Player& player)
{
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::hanging_Idle);

    this->nowAnimState.PlayTime_anim = 0.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
    this->nowAnimState.TotalPlayTime_anim = MV1GetAttachAnimTotalTime(modelHandle, this->nowAnimState.AttachIndex);

    //胴体座標
    VECTOR centerPosition = MV1GetFramePosition(modelHandle, 2);

    player.playerCalculation->Reset_move();
    player.SetMoveDirection_now(player.playerCalculation->HangingDirection(player.playerCalculation->GetHangingPoly(), centerPosition));
    player.SetRotata_x(0.0f);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> Hanging_Idle::Update(const VECTOR& cameraDirection,
    const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)
{
    VECTOR moveDirection = player.GetMoveDirection_now();

    PlayerData playerData = player.GetData();
    playerData.isGround = true;

    //上に登る
    if (PadInput::isUp())
    {
        playerData.isHang_to_Crouch = true;
        player.SetIsCollisionCheck(false);
        isChangeState = true;
    }

    //降りる
    if (PadInput::isDown())
    {
        playerData.isFalling = true;
        isChangeState = true;
    }

    return std::make_pair(moveDirection, playerData);
}

VECTOR Hanging_Idle::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //moveDirを取得する
    moveDirection = Move(cameraDirection, playerData);
    JumpMove(playerData, player);
    RollMove(playerData);

    return moveDirection;
}

void Hanging_Idle::Enter(PlayerData& playerData)
{
    playerData.isHanging = true;
}

void Hanging_Idle::Exit(PlayerData& playerData)
{
    playerData.isHanging = false;
}