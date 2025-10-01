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
Jump::Jump(const int modelHandle) :
    PlayerStateBase(modelHandle),
    isFirstJump(false),
    isSecondJump(false)
{
    
}

/// <summary>
/// デストラクタ
/// </summary>
Jump::~Jump()
{
  //  MV1DetachAnim(modelHandle, this->nowAnimState.attachIndex);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="modelHandle"></param>
void Jump::Initialize(const int modelHandle,const int changeNum, Player& player)
{
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    //ランジャンプ
    player.GetData().isMove ?
        PlayerStateBase::Initialize(modelHandle, animNum::runJump, player) :
        PlayerStateBase::Initialize(modelHandle, animNum::jump, player);

    this->nowAnimState.PlayAnimSpeed = kPlayAnimSpeed;

    isFirstJump = player.GetData().isJump;
    isSecondJump = player.GetData().isSecondJump;
}

/// <summary>
/// 更新
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> Jump::Update(
    const VECTOR& cameraDirection,
    const std::vector<std::weak_ptr<BaseObject>>& fieldObjects,
    Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //データを取得
    PlayerData playerData = player.GetData();

    moveDirection = Command(cameraDirection, playerData, player);

    //接地した場合、着地アニメーションに切り替え
    ResetIsJumps(playerData);

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

    if (nowAnimState.attachIndex != -1)
    {

        // アタッチしたアニメーションの総再生時間を取得する
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.attachIndex);

        //再生時間更新
        nowAnimState.playAnimTime += nowAnimState.PlayAnimSpeed;

        //総再生時間を超えたらリセット
        if (nowAnimState.playAnimTime >= totalTime_anim)
        {
            playerData.isFalling = true;
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

        // 再生時間を進める
        oldAnimState.playAnimTime += oldAnimState.PlayAnimSpeed;

        // 再生時間が総時間に到達していたら再生時間をループさせる
        if (oldAnimState.playAnimTime > totalTime_anim)
        {
            oldAnimState.playAnimTime = static_cast<float>(fmod(oldAnimState.playAnimTime, totalTime_anim));
        }

        // 変更した再生時間をモデルに反映させる
        MV1SetAttachAnimTime(modelHandle, oldAnimState.attachIndex, oldAnimState.playAnimTime);

        // アニメーション２のモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.attachIndex, 1.0f - animBlendRate);
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

void Jump::Exit(PlayerData& playerData)
{
    playerData.isJump = false;
    playerData.isWalljump = false;
}