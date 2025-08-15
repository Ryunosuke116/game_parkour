#include "common.h"
#include <memory>
#include <string>
#include "BaseScene.h"
#include "Result.h"
#include "PadInput.h"

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
    coinHandle = LoadGraph("material/png/coin_2d.png");
    crossHandle = LoadGraph("material/png/number_font/x_32x64.png");
    LoadDivGraph("material/png/suuji32x64_07.png",
        10, 10, 1, 32, 64, numberHandle);
    modelHandle = LoadGraph("material/png/title.png");
    blackOut = std::make_shared<BlackOut>();

    if (coinCount <= 20)
    {
        rankHandle = LoadGraph("material/png/rank_D.png");
    }
    else if (coinCount <= 40)
    {
        rankHandle = LoadGraph("material/png/rank_C.png");
    }
    else if (coinCount <= 60)
    {
        rankHandle = LoadGraph("material/png/rank_B.png");
    }
    else if (coinCount <= 90)
    {
        rankHandle = LoadGraph("material/png/rank_A.png");
    }
    else if (coinCount == 100)
    {
        rankHandle = LoadGraph("material/png/rank_S.png");
    }
}

/// <summary>
/// 初期化
/// </summary>
void Result::Initialize()
{
    num = std::to_string(coinCount);

    //一文字しか入ってない場合先頭に0を挿入する
    if (num.length() == 1)
    {
        num.insert(0, "0");
    }
    coin_x = 30;
    coin_y = 700;
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
            ChangeScene("Title",0);
        }
    }
}

/// <summary>
/// 描画
/// </summary>
void Result::Draw()
{
    //DrawGraph(0, 0, modelHandle, TRUE);
    //blackOut->Draw();

    DrawGraph(coin_x, coin_y, coinHandle, TRUE);

    int num_x = 170;

    for (char c : num)
    {
        int digit = c - '0';
        DrawGraph(num_x, 730, numberHandle[digit], TRUE);
        num_x += 32;
    }
    DrawGraph(400, 150, rankHandle, TRUE);

    DrawGraph(135, 730, crossHandle, TRUE);
}