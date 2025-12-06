#include "Common.h"
#include <memory>
#include <vector>
#include "PlayerStateBase.h"
#include "PadInput.h"
#include "PlayerData.h"
#include "QuickRoll.h"
#include "AnimNum.h"
#include "Player.h"
#include "PlayerCalculation.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldAnimState"></param>
/// <param name="nowAnimState"></param>
QuickRoll::QuickRoll(const int modelHandle) :
	PlayerStateBase(modelHandle)
{
    this->nowAnimState.playAnimSpeed = kPlayAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
QuickRoll::~QuickRoll()
{
    //処理なし
}

/// <summary>
/// 更新
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> QuickRoll::Update(const VECTOR& cameraDirection,
    const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    PlayerData playerData = player.GetData();

    moveDirection = Command(cameraDirection, playerData, player);

    return std::make_pair(moveDirection, playerData);
}

VECTOR QuickRoll::Command(const VECTOR& cameraDirection, 
    PlayerData& playerData, 
    Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    ResetIsJumps(playerData);

    //moveDirを取得する
    moveDirection = Move(cameraDirection, playerData);
    JumpMove(playerData, player);

    if (VSize(moveDirection) != 0.0f)
    {
        isRun = true;
        isIdle = false;
    }
    else
    {
        isRun = false;
        isIdle = true;
    }

    return moveDirection;
}


/// <summary>
/// アニメーション更新
/// </summary>
/// <param name="playerData"></param>
/// <returns></returns>
bool QuickRoll::MotionUpdate(PlayerData& playerData)
{
    float totalPlayAnimTime;

    //空中にいるときのアニメーション最大フレーム数
    const float kWhenAirMaxAnimTime = 40.0f;

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
        //再生時間更新
        nowAnimState.playAnimTime += nowAnimState.playAnimSpeed;

        if (nowAnimState.playAnimTime >= kWhenAirMaxAnimTime)
        {
            if (!playerData.isGround)
            {
                isChangeState = true;
                playerData.isFalling = true;
            }
        }

        //総再生時間を超えたらリセット
        if (nowAnimState.playAnimTime >= nowAnimState.totalPlayAnimTime)
        {
            playerData.isRun = isRun;
            playerData.isIdle = isIdle;
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
        totalPlayAnimTime = MV1GetAttachAnimTotalTime(modelHandle, oldAnimState.attachIndex);

        // 再生時間を進める
        oldAnimState.playAnimTime += oldAnimState.playAnimSpeed;

        // 再生時間が総時間に到達していたら再生時間をループさせる
        if (oldAnimState.playAnimTime > totalPlayAnimTime)
        {
            oldAnimState.playAnimTime = static_cast<float>(fmod(oldAnimState.playAnimTime, totalPlayAnimTime));
        }

        // 変更した再生時間をモデルに反映させる
        MV1SetAttachAnimTime(modelHandle, oldAnimState.attachIndex, oldAnimState.playAnimTime);

        // アニメーション２のモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.attachIndex, 1.0f - animBlendRate);
    }

    return true;
}

void QuickRoll::Exit(PlayerData& playerData)
{
    PlayerStateBase::Exit(playerData);
    playerData.isRoll = false;
}