#include "common.h"
#include <algorithm>
#include "UI_coin.h"
#include "JsonManager.h"

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

	for (auto& data : jsonData["coin"])
	{
		std::string path = data[0];
		std::string name = data[1];

		uiPath[name] = path;
	}

	coinHandle = LoadGraph(uiPath.at("coin").c_str());
	crossHandle = LoadGraph(uiPath.at("cross").c_str());
	LoadDivGraph(uiPath.at("number").c_str(),
		10, 10, 1, 480, 500, numberHandle);
}

void UI_coin::Create()
{
	Load(JsonManager::GetInstance().GetJsons("png"));
}

/// <summary>
/// 初期化
/// </summary>
void UI_coin::Initialize()
{
	//描画位置
	const int coin_x = 20;
	const int coin_y = 690;
	const int addInitCrossX = 135;
	const int addInitCrossY = 70;
	const int addInitNumberX = 190;
	const int addInitNumberY = 40;
	const std::string initNumber = "00";

	x = coin_x;
	y = coin_y;
	cross_x = x + addInitCrossX;
	cross_y = y + addInitCrossY;
	number_x = x + addInitNumberX;
	number_y = y + addInitNumberY;

	coinWidth = 140;
	coinHeight = 152;
	crossWidth = 60;
	crossHeight = 60;
	numberWidth = 100;
	numberHeight = 100;
	addNumberX = 70;

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
	//コインイラスト描画
	DrawExtendGraph(x, y, 
		x + coinWidth, y + coinHeight, 
		coinHandle, TRUE);
	//DrawGraph(x, y, coinHandle, TRUE);
	
	//×描画
	DrawExtendGraph(cross_x, cross_y,
		cross_x + crossWidth, cross_y + crossHeight,
		crossHandle, TRUE);
	//DrawGraph(cross_x, cross_y, crossHandle, TRUE);

	int num_x = number_x;

	//コイン所持数の桁数分、大きい桁から順に描画
	for (char c : countNumber)
	{
		int digit = c - '0';
		DrawExtendGraph(num_x, number_y,
			num_x + numberWidth, number_y + numberHeight,
			numberHandle[digit], TRUE);
		//DrawGraph(num_x, number_y, numberHandle[digit], TRUE);

		//文字の幅分ずらす
		num_x += addNumberX;
	}
	
}

/// <summary>
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void UI_coin::ResultCreate(const int coinCount)
{
	this->coinCount = coinCount;
	Load(JsonManager::GetInstance().GetJsons("png"));
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void UI_coin::ResultInitialize()
{
	const int coin_x = 960;
	const int coin_y = 630;
	const int addInitCrossX = 205;
	const int addInitCrossY = 110;
	const int addInitNumberX = 260;
	const int addInitNumberY = 60;
	const std::string initNumber = "00";

	x = coin_x;
	y = coin_y;
	cross_x = x + addInitCrossX;
	cross_y = y + addInitCrossY;
	number_x = x + addInitNumberX;
	number_y = y + addInitNumberY;

	coinWidth = 200;
	coinHeight = 212;
	crossWidth = 76;
	crossHeight = 76;
	numberWidth = 150;
	numberHeight = 150;
	addNumberX = 120;

	countNumber = initNumber;
	coinCount = 0;
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void UI_coin::ResultUpdate()
{
	Update();
}