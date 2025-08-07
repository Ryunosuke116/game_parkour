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
	modelHandle = LoadGraph("material/png/title.png");
	blackOut = std::make_shared<BlackOut>();
}

/// <summary>
/// デストラクタ
/// </summary>
Title::~Title()
{

}

/// <summary>
/// 初期化
/// </summary>
void Title::Initialize()
{
	blackOut->Initialize();
    PadInput::Initialize();
	isPush = false;
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
    }

    if (isPush)
    {
        blackOut->BlackOutUpdate(4.5f);
        if (blackOut->GetAlpha() >= 300)
        {
            ChangeScene("Game");
        }
    }
}

void Title::Draw()
{
	DrawGraph(0, 0, modelHandle, TRUE);
    blackOut->Draw();
}
