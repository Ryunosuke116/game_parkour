#include "common.h"
#include <memory>
#include <vector>
#include "PadInput.h"
#include "PlayerData.h"
#include "PlayerStateBase.h"
#include "HangingIdle.h"
#include "AnimTime.h"
#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldAnimState"></param>
/// <param name="nowAnimState"></param>
/// <param name="playerData"></param>
HangingIdle::HangingIdle(int& modelHandle,
    AnimState& oldAnimState, AnimState& nowAnimState) :
    PlayerStateBase(modelHandle, oldAnimState, nowAnimState)
{
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
HangingIdle::~HangingIdle()
{

}

/// <summary>
/// 初期化
/// </summary>
/// <param name="modelHandle"></param>
void HangingIdle::Initialize(int& modelHandle,const int changeNum, Player& player)
{
    PlayerStateBase::Initialize(modelHandle, changeNum, player);
    animBlendRate = 1.0f;

    // 再生時間をセットする
    MV1SetAttachAnimTime(modelHandle, nowAnimState.AttachIndex, nowAnimState.PlayTime_anim);
    //アニメーションのモデルに対する反映率をセット
    MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.AttachIndex, animBlendRate);

    //胴体座標
    VECTOR centerPosition = MV1GetFramePosition(modelHandle, 2);

    player.playerCalculation->Reset_move();
    player.SetNowMoveDirection(player.playerCalculation->HangingDirection(centerPosition));
    player.SetRotata_x(0.0f);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> HangingIdle::Update(const VECTOR& cameraDirection,
    const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)
{
    
    VECTOR moveDirection = player.GetNowMoveDirection();

    PlayerData playerData = player.GetData();
    playerData.isHanging_now = true;
    playerData.isGround = true;

    //上に登る
    if (PadInput::isUp())
    {
        playerData.isHangToCrouch = true;
        player.SetIsCollisionCheck(false);
        player.playerCalculation->ResetMoveVec_old();
        isChangeState = true;
        playerData.isJumpFirst = false;
        playerData.isJumpSecond = false;
        playerData.isJumpAll = false;
        playerData.isHanging_now = false;
    }

    //降りる
    if (PadInput::isDown())
    {
        isChangeState = true;
        playerData.isFalling = true;
        playerData.isHanging_now = false;
        playerData.isUseHanging = false;
    }

    return std::make_pair(moveDirection, playerData);
}

VECTOR HangingIdle::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //moveDirを取得する
    moveDirection = Move(cameraDirection, playerData);
    JumpMove(playerData, player);
    RollMove(playerData);

    return moveDirection;
}

void HangingIdle::Enter(PlayerData& playerData)
{
    playerData.isHanging = true;
}

void HangingIdle::Exit(PlayerData& playerData)
{
    playerData.isHanging = false;
}