#include "common.h"
#include <memory>
#include <string>
#include "BaseScene.h"
#include "Result.h"
#include "PadInput.h"
#include "JsonManager.h"
#include "Result_object.h"
#include "BlackOut.h"
#include "GameInstanceSubSystem.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="manager"></param>
Result::Result(SceneManager& manager) : BaseScene{ manager },
modelHandle(-1),
coinHandle(-1),
crossHandle(-1),
rankHandle(-1),
num(""),
coin_x(-1),
coin_y(-1),
isPush(false)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Result::~Result()
{

}

void Result::Create()
{
    const std::string jsonFileName = "JsonResult";
    JsonManager::GetInstance().Create(jsonFileName);
    GameInstanceSubSystem::GetInstance().Load(jsonFileName);

    objectManager = std::make_shared<ObjectManager>();
    objectManager->ResultCreate(coinCount);
}

/// <summary>
/// 初期化
/// </summary>
void Result::Initialize()
{
    BlackOut::GetInstance().ResultInitialize();

    objectManager->ResultInitilize();
    isPush = false;

    const auto soundPlayer =
        GameInstanceSubSystem::GetInstance().GetSubSystem<SoundPlayer>().lock();
    soundPlayer->Play("resultBGM");
}

/// <summary>
/// 更新処理
/// </summary>
void Result::Update()
{
    const int kMaxAlpha = 300;

    PadInput::Update();

    objectManager->ResultUpdate();

    if (BlackOut::GetInstance().GetAlpha() >= kMaxAlpha)
    {
        const auto soundPlayer =
            GameInstanceSubSystem::GetInstance().GetSubSystem<SoundPlayer>().lock();
        soundPlayer->Stop("resultBGM");
        ChangeScene("Title", 0);
    }
}

/// <summary>
/// 描画
/// </summary>
void Result::Draw()
{
    objectManager->ResultDraw();
    BlackOut::GetInstance().Draw();
}
