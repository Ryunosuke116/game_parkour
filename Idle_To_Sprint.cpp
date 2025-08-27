#include "common.h"
#include <memory>
#include <vector>
#include "PlayerStateBase.h"
#include "PadInput.h"
#include "PlayerData.h"
#include "Idle_To_Sprint.h"
#include "AnimTime.h"
#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Idle_To_Sprint::Idle_To_Sprint(int& modelHandle,
    AnimState& oldAnimState, AnimState& nowAnimState) :
    PlayerStateBase(modelHandle, oldAnimState, nowAnimState)
{
   
}

/// <summary>
/// デストラクタ
/// </summary>
Idle_To_Sprint::~Idle_To_Sprint()
{
    //  MV1DetachAnim(modelHandle, this->nowAnimState.AttachIndex);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="modelHandle"></param>
void Idle_To_Sprint::Initialize(int& modelHandle,const int changeNum, Player& player)
{
    PlayerStateBase::Initialize(modelHandle, changeNum, player);
    animBlendRate = 1.0f;
}

/// <summary>
/// 更新
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> Idle_To_Sprint::Update(const VECTOR& cameraDirection,
    const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    PlayerData playerData = player.GetData();

    moveDirection = Command(cameraDirection, playerData, player);

    return std::make_pair(moveDirection, playerData);
}

VECTOR Idle_To_Sprint::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //moveDirを取得する
    moveDirection = Move(cameraDirection, playerData);
    JumpMove(playerData, player);
    RollMove(playerData);

    if (VSize(moveDirection) == 0.0f && !playerData.isRoll)
    {
        playerData.isIdle = true;

    }

    return moveDirection;
}

bool Idle_To_Sprint::MotionUpdate(PlayerData& playerData)
{
    float totalTime_anim;

    // ブレンド率が１以下の場合は１に近づける
    if (animBlendRate < 1.0f)
    {
        animBlendRate += AnimBlendSpeed;
        if (animBlendRate > 1.0f)
        {
            animBlendRate = 1.0f;
        }
    }

    if (nowAnimState.AttachIndex != -1)
    {
        // アタッチしたアニメーションの総再生時間を取得する
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.AttachIndex);

        //再生時間更新
        nowAnimState.PlayTime_anim += nowAnimState.PlayAnimSpeed;

        //総再生時間を超えたらリセット
        if (nowAnimState.PlayTime_anim >= 6.0f)
        {
            playerData.isRun = true;
        }

        // 再生時間をセットする
        MV1SetAttachAnimTime(modelHandle, nowAnimState.AttachIndex, nowAnimState.PlayTime_anim);

        //アニメーションのモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.AttachIndex, animBlendRate);
    }


    //再生しているアニメーション２の処理
    if (oldAnimState.AttachIndex != -1)
    {
        // アニメーションの総時間を取得
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, oldAnimState.AttachIndex);

        // 再生時間を進める
       // oldAnimState.PlayTime_anim += oldAnimState.PlayAnimSpeed;

        // 再生時間が総時間に到達していたら再生時間をループさせる
        if (oldAnimState.PlayTime_anim > totalTime_anim)
        {
            //oldAnimState.PlayTime_anim = static_cast<float>(fmod(oldAnimState.PlayTime_anim, totalTime_anim));
        }

        // 変更した再生時間をモデルに反映させる
        MV1SetAttachAnimTime(modelHandle, oldAnimState.AttachIndex, oldAnimState.PlayTime_anim);

        // アニメーション２のモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.AttachIndex, 1.0f - animBlendRate);
    }

    return false;
}

void Idle_To_Sprint::Enter(PlayerData& playerData)
{
    playerData.isSprint = true;
}

void Idle_To_Sprint::Exit(PlayerData& playerData)
{
    playerData.isSprint = false;
}