#include "common.h"
#include <memory>
#include <vector>
#include "PlayerStateActionBase.h"
#include "PlayerData.h"
#include "Idle.h"
#include "AnimTime.h"
#include "Player.h"
#include "PadInput.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Idle::Idle(int& modelHandle,
    AnimState& oldAnimState, AnimState& nowAnimState, std::shared_ptr<ISoundPlayer> sound, PlayerData& playerData) :
    PlayerStateActionBase(modelHandle, oldAnimState, nowAnimState, sound)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Idle::~Idle()
{
  //  MV1DetachAnim(modelHandle, this->nowAnimState.AttachIndex);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="modelHandle"></param>
void Idle::Initialize(int& modelHandle, Player& player)
{
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::idle);

    this->nowAnimState.PlayTime_anim = 0.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// 更新
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> Idle::Update(const VECTOR& cameraDirection,
    const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    PlayerData playerData = player.GetData();

    moveDirection = Command(cameraDirection, playerData, player);

    return std::make_pair(moveDirection, playerData);
}

VECTOR Idle::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //moveDirを取得する
    moveDirection = Move(cameraDirection, playerData);
    JumpMove(playerData, player);
    RollMove(playerData);

    if (VSize(moveDirection) != 0.0f && !playerData.isRoll)
    {
        playerData.isRun = true;
        isChangeState = true;
    }

    return moveDirection;
}

void Idle::Enter(PlayerData& playerData)
{
    playerData.isIdle = true;
}

void Idle::Exit(PlayerData& playerData)
{
    playerData.isIdle = false;
}
