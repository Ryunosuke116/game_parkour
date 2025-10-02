#include "common.h"
#include <memory>
#include <vector>
#include "PlayerStateBase.h"
#include "PlayerData.h"
#include "RunToStop.h"
#include "AnimTime.h"
#include "Player.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
RunToStop::RunToStop(const int modelHandle) :
    PlayerStateBase(modelHandle)
{
    this->nowAnimState.PlayAnimSpeed = kPlayAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
RunToStop::~RunToStop()
{
    //  MV1DetachAnim(modelHandle, this->nowAnimState.attachIndex);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> RunToStop::Update(const VECTOR& cameraDirection,
    const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    PlayerData playerData = player.GetData();

    moveDirection = Command(cameraDirection, playerData, player);

    return std::make_pair(moveDirection, playerData);
}

VECTOR RunToStop::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //moveDirを取得する
    moveDirection = Move(cameraDirection, playerData);
    JumpMove(playerData, player);
    RollMove(playerData);

    if (VSize(moveDirection) != 0.0f && !playerData.isRoll)
    {
        playerData.isRun = true;
        playerData.isStopRun = false;
        isChangeState = true;
    }

    return moveDirection;
}


/// <summary>
/// アニメーション更新
/// </summary>
/// <returns></returns>
bool RunToStop::MotionUpdate(PlayerData& playerData)
{
    float totalTime_anim;

    // ブレンド率が１以下の場合は１に近づける
    if (animBlendRate < 1.0f)
    {
        animBlendRate += kAnimBlendSpeed;
        if (animBlendRate > 1.0f)
        {
            animBlendRate = 1.0f;
        }
    }

    if (nowAnimState.attachIndex != -1)
    {
        // アタッチしたアニメーションの総再生時間を取得する
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.attachIndex);

        //再生時間更新
        nowAnimState.playAnimTime += nowAnimState.PlayAnimSpeed;

        //総再生時間を超えたらリセット
        if (nowAnimState.playAnimTime >= totalTime_anim)
        {
            playerData.isIdle = true;
            isChangeState = true;
        }

        // 再生時間をセットする
        MV1SetAttachAnimTime(modelHandle, nowAnimState.attachIndex, nowAnimState.playAnimTime);

        //アニメーションのモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.attachIndex, animBlendRate);
    }


    //再生しているアニメーション２の処理
    if (oldAnimState.attachIndex != -1)
    {
        // アニメーションの総時間を取得
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, oldAnimState.attachIndex);

        // 変更した再生時間をモデルに反映させる
        MV1SetAttachAnimTime(modelHandle, oldAnimState.attachIndex, oldAnimState.playAnimTime);

        // アニメーション２のモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.attachIndex, 1.0f - animBlendRate);
    }

    if (!playerData.isGround)
    {
        return true;
    }

    return false;
}

void RunToStop::Exit(PlayerData& playerData)
{
    playerData.isStopRun = false;
}