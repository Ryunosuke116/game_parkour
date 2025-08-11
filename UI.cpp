#include "common.h"
#include <string>
#include "UI.h"

/// <summary>
/// コンストラクタ
/// </summary>
UI::UI()
{
	coinHandle = LoadGraph("material/png/coin_2d.png");
	crossHandle = LoadGraph("material/png/number_font/x_32x64.png");
	LoadDivGraph("material/png/number_font/suuji32x64_07.png",
		10, 10, 1, 32, 64, numberHandle);
}

/// <summary>
/// デストラクタ
/// </summary>
UI::~UI()
{

}

/// <summary>
/// 初期化
/// </summary>
void UI::Initialize()
{
	x = 30;
	y = 700;
	num = "00";
	coinCount = 0;
}

/// <summary>
/// 更新
/// </summary>
void UI::Update()
{
	num = std::to_string(coinCount);

	//一文字しか入ってない場合先頭に0を挿入する
	if (num.length() == 1)
	{
		num.insert(0, "0");
	}
}

void UI::Draw()
{
	DrawGraph(x, y, coinHandle, TRUE);

	int num_x = 170;


	for (char c : num)
	{
		int digit = c - '0';
		DrawGraph(num_x, 730, numberHandle[digit], TRUE);
		num_x += 32;
	}



	DrawGraph(135, 730, crossHandle, TRUE);

}