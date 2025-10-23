#include "Common.h"
#include <memory>
#include <vector>
#include "PlayerStateBase.h"
#include "PadInput.h"
#include "PlayerData.h"
#include "Victory.h"
#include "AnimTime.h"
#include "Player.h"

/// @brief コンストラクタ
/// @param modelHandle 
/// @param oldAnimState 
/// @param nowAnimState 
/// @param playerData 
Victory::Victory(const int modelHandle) :
    PlayerStateBase(modelHandle)
{
    this->nowAnimState.PlayAnimSpeed = kPlayAnimSpeed;
}

/// @brief デストラクタ
Victory::~Victory()
{
    //処理なし
}

/// @brief 更新処理
std::pair<VECTOR, PlayerData> Victory::Update(
    const VECTOR& cameraDirection,
    const std::vector<std::weak_ptr<BaseObject>>& fieldObjects,
    Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //データを取得
    PlayerData playerData = player.GetData();

    return std::make_pair(moveDirection, playerData);
}

bool Victory::MotionUpdate(PlayerData& playerData)
{
    const float kPoseFrame = 32.0f;
    
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
        nowAnimState.playAnimTime += nowAnimState.PlayAnimSpeed;

        //32f(決めポーズ時)になったらそこでアニメーションを停止する
        if (nowAnimState.playAnimTime >= kPoseFrame)
        {
            nowAnimState.playAnimTime = kPoseFrame;
        }

        // 再生時間をセットする
        MV1SetAttachAnimTime(modelHandle, nowAnimState.attachIndex, nowAnimState.playAnimTime);

        //アニメーションのモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.attachIndex, animBlendRate);
    }

    return true;
}

VECTOR Victory::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    return moveDirection;
}

void Victory::Exit(PlayerData& playerData)
{
    PlayerStateBase::Exit(playerData);
    playerData.isVictory = false;
}