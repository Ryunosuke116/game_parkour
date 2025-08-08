#include "common.h"
#include <memory>
#include "BaseScene.h"
#include "Result.h"
#include "PadInput.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="manager"></param>
Result::Result(SceneManager& manager) : BaseScene{ manager },
modelHandle(-1)
{
	modelHandle = LoadGraph("material/png/title.png");
	blackOut = std::make_shared<BlackOut>();
}

/// <summary>
/// デストラクタ
/// </summary>
Result::~Result()
{

}

/// <summary>
/// 初期化
/// </summary>
void Result::Initialize()
{
  
}

/// <summary>
/// 更新処理
/// </summary>
void Result::Update()
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
            ChangeScene("Title");
        }
    }
}

/// <summary>
/// 描画
/// </summary>
void Result::Draw()
{
    DrawGraph(0, 0, modelHandle, TRUE);
    blackOut->Draw();
}