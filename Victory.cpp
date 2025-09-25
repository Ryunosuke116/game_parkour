#include "common.h"
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
Victory::Victory(int& modelHandle, AnimState& oldAnimState,
    AnimState& nowAnimState, PlayerData& playerData) :
    PlayerStateBase(modelHandle, oldAnimState, nowAnimState)
{
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
}

/// @brief デストラクタ
Victory::~Victory()
{

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
    bool flag = false;

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
        float poseFrame = 32.0f;

        //再生時間更新
        nowAnimState.PlayTime_anim += nowAnimState.PlayAnimSpeed;

        //32f(決めポーズ時)になったらそこでアニメーションを停止する
        if (nowAnimState.PlayTime_anim >= poseFrame)
        {
            nowAnimState.PlayTime_anim = poseFrame;
        }

        // 再生時間をセットする
        MV1SetAttachAnimTime(modelHandle, nowAnimState.AttachIndex, nowAnimState.PlayTime_anim);

        //アニメーションのモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.AttachIndex, animBlendRate);
    }
    return flag;
}

VECTOR Victory::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    return moveDirection;
}

void Victory::Enter(PlayerData& playerData)
{
    playerData.isVictory = true;
}

void Victory::Exit(PlayerData& playerData)
{
    playerData.isVictory = false;
}