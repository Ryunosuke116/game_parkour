#include "Dxlib.h"
#include "PlayerData.h"
#include "PlayerEffectController.h"
#include "GameInstanceSubSystem.h"
#include "WorldSubSystem.h"
#include "EffectManager.h"
#include "RankScoreUi.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerEffectController::PlayerEffectController():
    effectTimer(-1.0f)
{
    //処理なし
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerEffectController::~PlayerEffectController()
{
    //処理なし
}

/// <summary>
/// 生成時に行う処理
/// </summary>
void PlayerEffectController::Create()
{
    //エフェクトマネージャーのポインタを参照
    wpEffectManager = GameInstanceSubSystem::GetInstance().GetSubSystem<EffectManager>();
}

void PlayerEffectController::Initialize()
{
    effectTimer = 0.0f;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="playerPos"></param>
/// <param name="playerData"></param>
void PlayerEffectController::Update(const VECTOR& playerPos,
    const PlayerData& playerData)
{
    const float     kMaxEffectTimer     = 10.0f;
    const float     kAddEffectPositionY = 2.0f;
    const VECTOR    kEffectScale        = VGet(4.0f, 4.0f, 4.0f);

    VECTOR effectPosition = playerPos;
    effectPosition.y += kAddEffectPositionY;

    if (playerData.isRun)
    {
        effectTimer++;

        if (effectTimer >= kMaxEffectTimer)
        {
            wpEffectManager.lock()->PlayEffect("footSmoke");
            wpEffectManager.lock()->SetScale(kEffectScale, "footSmoke");
            wpEffectManager.lock()->SetPosition(effectPosition, "footSmoke");

            effectTimer = 0.0f;
        }
    }

    effectPosition.y += kAddEffectPositionY;

    //スコアランクに応じてエフェクトの色を変える
    ChangeColorByScoreRank();
    wpEffectManager.lock()->SetPosition(effectPosition, "playerbuff");
    wpEffectManager.lock()->SetPosition(effectPosition, "coinTouch");
}

/// <summary>
/// スコアランクに応じてエフェクトの色を変更する
/// </summary>
void PlayerEffectController::ChangeColorByScoreRank()
{
    const auto& rankScoreUi = WorldSubSystem::GetInstance().GetSubSystem<RankScoreUi>();

    //エフェクトを再生する
    if (wpEffectManager.lock()->GetIsPlayEffect("playerbuff") &&
        rankScoreUi->GetRankHandleKey() != "")
    {
        wpEffectManager.lock()->PlayEffect("playerbuff");
    }

    if (rankScoreUi->GetIsChangeRank())
    {
        if (rankScoreUi->GetRankHandleKey() == "S")
        {
            wpEffectManager.lock()->StopEffect("playerbuff");
        }
        else if (rankScoreUi->GetRankHandleKey() == "A")
        {
            wpEffectManager.lock()->SetEffectColor("playerbuff", scoreAColor);
        }
        else if (rankScoreUi->GetRankHandleKey() == "B")
        {
            wpEffectManager.lock()->SetEffectColor("playerbuff", scoreBColor);
        }
        else if (rankScoreUi->GetRankHandleKey() == "C")
        {
            wpEffectManager.lock()->SetEffectColor("playerbuff", scoreCColor);
        }
        else if (rankScoreUi->GetRankHandleKey() == "D")
        {
            wpEffectManager.lock()->SetEffectColor("playerbuff", scoreDColor);
        }
        else if (rankScoreUi->GetRankHandleKey() == "")
        {
            wpEffectManager.lock()->StopEffect("playerbuff");
        }
    }
}

/// <summary>
/// コインを獲得したときのエフェクトを再生する
/// </summary>
void PlayerEffectController::PlayCoinTouchEffect()
{
    //エフェクトを再生する
    if (wpEffectManager.lock()->GetIsPlayEffect("coinTouch"))
    {
        wpEffectManager.lock()->PlayEffect("coinTouch");
    }
}