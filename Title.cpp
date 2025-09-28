#include "common.h"
#include "SceneManager.h"
#include "BaseScene.h"
#include "TitleObjectManager.h"
#include "Title.h"
#include "PadInput.h"
#include "JsonManager.h"
#include "GameInstanceSubSystem.h"
#include "SoundPlayer.h"

class Game;

/// <summary>
/// インスタンス化
/// </summary>
/// <param name="manager"></param>
Title::Title(SceneManager& manager) : BaseScene{ manager },
modelHandle(-1)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Title::~Title()
{
 
}

void Title::Create()
{
    // 1,Jsonファイル読み込み
    // 2,サブシステムを追加
    // 3,各objectを生成
    // 4,各objectをcreate処理をする

    const std::string jsonFileName = "JsonTitle";

    objectManager = std::make_shared<TitleObjectManager>();

    JsonManager::GetInstance().Create(jsonFileName);
    GameInstanceSubSystem::GetInstance().Load(jsonFileName);

    objectManager->Create();
}

/// <summary>
/// 初期化
/// </summary>
void Title::Initialize()
{
    const auto soundPlayer = GameInstanceSubSystem::GetInstance().GetSubSystem<SoundPlayer>().lock();
	BlackOut::GetInstance().Initialize();
	isPush = false;
    soundPlayer->Play("titleBGM");
    objectManager->Initialize();
}

/// <summary>
/// 更新
/// </summary>
void Title::Update()
{
    const int addAlpha = 5;
    const int maxAlpha = 300;
    const auto soundPlayer = GameInstanceSubSystem::GetInstance().GetSubSystem<SoundPlayer>().lock();

    PadInput::Update();

    objectManager->Update();

    if (PadInput::IsPush_A() && !isPush)
    {
        isPush = true;
        soundPlayer->Play("button");
    }

    if (isPush)
    {
        BlackOut::GetInstance().BlackOutUpdate(addAlpha);
        if (BlackOut::GetInstance().GetAlpha() >= maxAlpha)
        {
            BlackOut::GetInstance().SetIsLightChange(true);
            soundPlayer->Stop("titleBGM");
            ChangeScene("Game",0);
        }
    }
}

void Title::Draw()
{
    objectManager->Draw();
    BlackOut::GetInstance().Draw();
}
