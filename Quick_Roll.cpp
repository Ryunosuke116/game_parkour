#include "common.h"
#include <memory>
#include <vector>
#include "PlayerStateBase.h"
#include "PadInput.h"
#include "PlayerData.h"
#include "Quick_Roll.h"
#include "AnimTime.h"
#include "Player.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldAnimState"></param>
/// <param name="nowAnimState"></param>
Quick_Roll::Quick_Roll(int& modelHandle,
    AnimState& oldAnimState, AnimState& nowAnimState, std::shared_ptr<ISoundPlayer> sound) :
	PlayerStateBase(modelHandle, oldAnimState, nowAnimState, sound)
{
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
Quick_Roll::~Quick_Roll()
{

}

/// <summary>
/// 更新
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> Quick_Roll::Update(const VECTOR& cameraDirection,
    const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    PlayerData playerData = player.GetData();

    moveDirection = Command(cameraDirection, playerData, player);

    return std::make_pair(moveDirection, playerData);
}

VECTOR Quick_Roll::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    FlagReset_jump(playerData);

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
bool Quick_Roll::MotionUpdate(PlayerData& playerData)
{
    float totalTime_anim;
    float flag = false;

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

        if (nowAnimState.PlayTime_anim >= 40.0f)
        {
            if (!playerData.isGround)
            {
                isChangeState = true;
                playerData.isFalling = true;
            }
        }

        //総再生時間を超えたらリセット
        if (nowAnimState.PlayTime_anim >= totalTime_anim)
        {
            playerData.isRun = isRun;
            playerData.isIdle = isIdle;
            isChangeState = true;
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
        oldAnimState.PlayTime_anim += oldAnimState.PlayAnimSpeed;

        // 再生時間が総時間に到達していたら再生時間をループさせる
        if (oldAnimState.PlayTime_anim > totalTime_anim)
        {
            oldAnimState.PlayTime_anim = static_cast<float>(fmod(oldAnimState.PlayTime_anim, totalTime_anim));
        }

        // 変更した再生時間をモデルに反映させる
        MV1SetAttachAnimTime(modelHandle, oldAnimState.AttachIndex, oldAnimState.PlayTime_anim);

        // アニメーション２のモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.AttachIndex, 1.0f - animBlendRate);
    }

    return flag;
}

void Quick_Roll::Enter(PlayerData& playerData)
{
    playerData.isJumpAll = false;
    playerData.isJump_second = false;
}

void Quick_Roll::Exit(PlayerData& playerData)
{
    playerData.isRoll = false;
}