#include "common.h"
#include <memory>
#include <vector>
#include "PlayerStateBase.h"
#include "PadInput.h"
#include "PlayerData.h"
#include "Jump.h"
#include "AnimTime.h"
#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Jump::Jump(int& modelHandle, AnimState& oldAnimState,
    AnimState& nowAnimState, std::shared_ptr<ISoundPlayer> sound, PlayerData& playerData) :
    PlayerStateBase(modelHandle, oldAnimState, nowAnimState, sound),
    isJump_first(false),
    isJump_second(false)
{
    
}

/// <summary>
/// デストラクタ
/// </summary>
Jump::~Jump()
{
  //  MV1DetachAnim(modelHandle, this->nowAnimState.AttachIndex);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="modelHandle"></param>
void Jump::Initialize(int& modelHandle,const int changeNum, Player& player)
{
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    //ランジャンプ
    player.GetData().isMove ?
        PlayerStateBase::Initialize(modelHandle, animNum::run_Jump, player) :
        PlayerStateBase::Initialize(modelHandle, animNum::jump, player);

    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;

    isJump_first = player.GetData().isJump;
    isJump_second = player.GetData().isJump_second;
}

/// <summary>
/// 更新
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> Jump::Update(const VECTOR& cameraDirection,
    const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //データを取得
    PlayerData playerData = player.GetData();

    moveDirection = Command(cameraDirection, playerData, player);

    //接地した場合、着地アニメーションに切り替え
    FlagReset_jump(playerData);

    if (playerData.isGround && !isRun)
    {
        playerData.isIdle = true;
        isChangeState = true;
    }
    else if (playerData.isGround && isRun)
    {
        playerData.isRun = true;
        isChangeState = true;
    }

    return std::make_pair(moveDirection, playerData);
}

/// <summary>
/// アニメーション更新 
/// </summary>
/// <returns></returns>
bool Jump::MotionUpdate(PlayerData& playerData)
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
            playerData.isFalling = true;
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

VECTOR Jump::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //moveDirを取得する
    moveDirection = Move(cameraDirection, playerData);
    JumpMove(playerData, player);
    RollMove(playerData);

    return moveDirection;
}

void Jump::Enter(PlayerData& playerData)
{
    playerData.isJump = true;
}

void Jump::Exit(PlayerData& playerData)
{
    playerData.isJump = false;
    playerData.isWalljump = false;
}