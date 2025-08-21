#include "common.h"
#include <memory>
#include "SceneManager.h"
#include "BaseScene.h"
#include "Title.h"
#include "PadInput.h"

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
    modelHandle = LoadGraph("material/png/title.png");
    soundHandle = LoadSoundMem("material/sound/titleBGM.mp3");
    buttonSound = LoadSoundMem("material/sound/button.mp3");
    titleHandle = LoadGraph("material/png/START_04.png");
    blackOut = std::make_shared<BlackOut>();
}

/// <summary>
/// 初期化
/// </summary>
void Title::Initialize()
{
	blackOut->Initialize();
	isPush = false;
    PlaySoundMem(soundHandle, DX_PLAYTYPE_LOOP);
}

/// <summary>
/// 更新
/// </summary>
void Title::Update()
{
    PadInput::Update();

    if (PadInput::IsPush_A() && !isPush)
    {
        isPush = true;
        PlaySoundMem(buttonSound, DX_PLAYTYPE_BACK);
    }

    if (isPush)
    {
        blackOut->BlackOutUpdate(4.5f);
        if (blackOut->GetAlpha() >= 300)
        {
            StopSoundMem(soundHandle);
            ChangeScene("Game",0);
        }
    }
}

void Title::Draw()
{
	DrawGraph(0, 0, modelHandle, TRUE);

    DrawGraph(300, 650, titleHandle, true);
    blackOut->Draw();
}
