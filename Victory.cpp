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

}

/// @brief デストラクタ
Victory::~Victory()
{

}

/// @brief 初期化
void Victory::Initialize(int& modelHandle, const int changeNum, Player& player)
{
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
}

/// @brief 更新処理
std::pair<VECTOR, PlayerData> Victory::Update(
    const VECTOR& cameraDirection,
    const std::vector<std::weak_ptr<BaseObject>>& fieldObjects,
    Player& player)
{

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

        // アタッチしたアニメーションの総再生時間を取得する
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.AttachIndex);

        //再生時間更新
        nowAnimState.PlayTime_anim += nowAnimState.PlayAnimSpeed;

        //総再生時間を超えたらリセット
        if (nowAnimState.PlayTime_anim >= totalTime_anim)
        {
            nowAnimState.PlayTime_anim = totalTime_anim;
        }

        // 再生時間をセットする
        MV1SetAttachAnimTime(modelHandle, nowAnimState.AttachIndex, nowAnimState.PlayTime_anim);

        //アニメーションのモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.AttachIndex, animBlendRate);
    }
}


void Victory::Enter(PlayerData& playerData)
{
    playerData.isVictory = true;
}

void Victory::Exit(PlayerData& playerData)
{
    playerData.isVictory = false;
}