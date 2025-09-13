#include "common.h"
#include <algorithm>
#include "UI_coin.h"


UI_coin::UI_coin() :
	BaseUI(),
	coinHandle(-1),
	crossHandle(-1),
	coinCount(-1),
	countNumber("")
{
	std::fill(
		std::begin(numberHandle),
		std::end(numberHandle),
		-1);

	jsonTag = "coin";
}

UI_coin::~UI_coin()
{
	for (auto& numberHandle : numberHandle)
	{
		DeleteGraph(numberHandle);
	}
	DeleteGraph(coinHandle);
	DeleteGraph(crossHandle);
}

void UI_coin::Load(const nlohmann::json& jsonData)
{
	std::unordered_map<std::string, std::string> uiPath;

	for (auto& data : jsonData)
	{
		std::string path = data[0];
		std::string name = data[1];

		uiPath[name] = path;
	}

	coinHandle = LoadGraph(uiPath.at("coin").c_str());
	crossHandle = LoadGraph(uiPath.at("cross").c_str());
	LoadDivGraph(uiPath.at("number").c_str(),
		10, 10, 1, 32, 64, numberHandle);
}

void UI_coin::Initialize()
{
	const int addInitCrossX = 105;
	const int addInitCrossY = 30;
	const int addInitNumberX = 140;
	const int addInitNumberY = 30;
	const std::string initNumber = "00";

	cross_x = x + addInitCrossX;
	cross_y = y + addInitCrossY;
	number_x = x + addInitNumberX;
	number_y = y + addInitNumberY;

	countNumber = initNumber;
	coinCount = 0;
}

void UI_coin::Update()
{
	countNumber = std::to_string(coinCount);

	//一文字しか入ってない場合先頭に0を挿入する
	if (countNumber.length() == 1)
	{
		countNumber.insert(0, "0");
	}
}

void UI_coin::Draw()
{
	const int addNumber_x = 32;

	DrawGraph(x, y, coinHandle, TRUE);

	int num_x = number_x;

	//コイン所持数の桁数分、大きい桁から順に描画
	for (char c : countNumber)
	{
		int digit = c - '0';
		DrawGraph(num_x, number_y, numberHandle[digit], TRUE);

		//文字の幅分ずらす
		num_x += addNumber_x;
	}

	DrawGraph(cross_x, cross_y, crossHandle, TRUE);
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void UI_coin::ResultInitialize()
{
	Initialize();
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void UI_coin::ResultUpdate()
{
	Update();
}