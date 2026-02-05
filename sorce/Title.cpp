#include "Common.h"
#include "SceneManager.h"
#include "BaseScene.h"
#include "ObjectManager.h"
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
Title::Title(SceneManager& manager) : BaseScene{ manager }
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

    objectManager = std::make_shared<ObjectManager>();

    JsonManager::GetInstance().Create(jsonFileName);
    GameInstanceSubSystem::GetInstance().Load(jsonFileName);

    objectManager->TitleCreate();
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
    objectManager->TitleInitilize();
}

/// <summary>
/// 更新
/// </summary>
void Title::Update()
{
    const int kAddAlpha = 5;
    const int kMaxAlpha = 300;
    const auto soundPlayer = GameInstanceSubSystem::GetInstance().GetSubSystem<SoundPlayer>().lock();

    PadInput::Update();

    objectManager->TitleUpdate();

    if (PadInput::IsPushA() && !isPush)
    {
        isPush = true;
        soundPlayer->Play("button");
    }

    if (isPush)
    {
        BlackOut::GetInstance().BlackOutUpdate(kAddAlpha);
        if (BlackOut::GetInstance().GetAlpha() >= kMaxAlpha)
        {
            objectManager->~ObjectManager();
            BlackOut::GetInstance().SetIsLightChange(true);
            soundPlayer->Stop("titleBGM");
            ChangeScene("Game",0);
        }
    }
}

void Title::Draw()
{
    objectManager->TitleDraw();
    BlackOut::GetInstance().Draw();
}
