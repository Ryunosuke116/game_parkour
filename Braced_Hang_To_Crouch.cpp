#include "common.h"
#include <memory>
#include <vector>
#include "PadInput.h"
#include "PlayerStateBase.h"
#include "Braced_Hang_To_Crouch.h"
#include "AnimTime.h"
#include "Player.h"
#include "HitCheck.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldAnimState"></param>
/// <param name="nowAnimState"></param>
/// <param name="playerData"></param>
Braced_Hang_To_Crouch::Braced_Hang_To_Crouch(int& modelHandle,
    AnimState& oldAnimState, AnimState& nowAnimState, std::shared_ptr<ISoundPlayer> sound) :
    PlayerStateBase(modelHandle, oldAnimState, nowAnimState, sound)
{
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
Braced_Hang_To_Crouch::~Braced_Hang_To_Crouch()
{

}

/// <summary>
/// 更新
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> Braced_Hang_To_Crouch::Update(const VECTOR& cameraDirection,
    const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)
{
    VECTOR moveVec = player.GetMoveDirection_now();

    PlayerData playerData = player.GetData();

    moveVec = Hang_to_CrouchMove(fieldObjects, player,playerData);

    return std::make_pair(moveVec, playerData);
}

VECTOR Braced_Hang_To_Crouch::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //moveDirを取得する
    moveDirection = Move(cameraDirection, playerData);

    return moveDirection;
}

void Braced_Hang_To_Crouch::Enter(PlayerData& playerData)
{
    playerData.isHang_to_Crouch = true;
}

void Braced_Hang_To_Crouch::Exit(PlayerData& playerData)
{
    playerData.isHang_to_Crouch = false;
}

/// <summary>
/// 登り
/// </summary>
/// <param name="mapHandle"></param>
VECTOR Braced_Hang_To_Crouch::Hang_to_CrouchMove(const std::vector<std::weak_ptr<BaseObject>>& fieldObjects,
    Player& player, PlayerData& playerData)
{
    //////////////////////////////////
    //  コード整理しろ！
    /////////////////////////////////

    VECTOR returnVec = VGet(0.0f, 0.0f, 0.0f);

    //指定のフレームまでは手に合わせて座標を更新
    if (nowAnimState.PlayTime_anim <= 22.0f)
    {
        VECTOR addPos = player.playerCalculation->HangingPosition();

        returnVec = addPos;
    }
    //
    else
    {
        //上り終わった後に少しずつ前進する
        returnVec = VScale(player.GetMoveDirection_now(), 0.35f);

        //胸の位置が床の位置をすぎたら足もとを基準に床との衝突判定をする
        VECTOR chestPos = MV1GetFramePosition(modelHandle, 6);

        VECTOR nowPos = chestPos;

        VECTOR newPos = VAdd(nowPos, returnVec);

        //足のフレーム座標で衝突判定
        VECTOR foot = MV1GetFramePosition(modelHandle, 167);

        auto result = GroundCollisionCheck_Hang_to_Crouch(fieldObjects, nowPos, foot, newPos);

        //playerの座標はフレーム座標を基準にしていないため縦だけずらす
        returnVec.y = result.second.y - nowPos.y;

        //上り終わったらplayerDataを初期化
        if (nowAnimState.PlayTime_anim >=
            nowAnimState.TotalPlayTime_anim - 1.0f)
        {
            isChangeState = true;
            player.SetIsCollisionCheck(true);
            if (!playerData.isMove)
            {
                playerData.isIdle = true;
            }
            else
            {
                playerData.isRun = true;
            }
            playerData.isGround = result.first;
        }
    }

    return returnVec;
}

/// <summary>
/// 床との衝突判定処理
/// 登るとき専用
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldPos"></param>
/// <param name="newPos"></param>
/// <param name="positionData.addTopPos"></param>
/// <param name="positionData.addBottomPos"></param>
/// <param name="positionData.radius"></param>
/// <param name="isJump"></param>
/// <returns></returns>
std::pair<bool, VECTOR> Braced_Hang_To_Crouch::GroundCollisionCheck_Hang_to_Crouch(
    const std::vector<std::weak_ptr<BaseObject>>& fieldObjects,
    const VECTOR& topPos, const VECTOR& foot, const VECTOR& newPos)
{

    bool isHitGround = false;

    VECTOR bottomPos = VGet(topPos.x, foot.y, topPos.z);
    //ごまかしで少し下にrayを伸ばす
    bottomPos.y -= 5.0f;

    MV1_COLL_RESULT_POLY rayPoly_ground;
    VECTOR returnPos = newPos;

    for (const auto& fieldObject : fieldObjects)
    {
        auto collisionObject = fieldObject.lock();

        //rayが当たっていれば
        isHitGround = HitCheck::RayHitJudge(collisionObject->GetModelHandle(), -1, topPos, bottomPos, rayPoly_ground);

        if (isHitGround)
        {
            VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);

            //床 - プレイヤーの足元で押し戻し量を計算
            newPlayerPos.y = rayPoly_ground.HitPosition.y - foot.y;
            returnPos.y = returnPos.y + newPlayerPos.y;
        }
    }

    //接地しているか
    return std::make_pair(isHitGround, returnPos);

}