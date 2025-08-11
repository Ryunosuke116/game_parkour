#include "common.h"
#include <algorithm>
#include "UI_coin.h"


UI_coin::UI_coin() :
	BaseUI(),
	coinHandle(-1),
	crossHandle(-1),
	coinCount(-1),
	num("")
{
	std::fill(
		std::begin(numberHandle),
		std::end(numberHandle),
		-1
	);

	jsonTag = "coin";
}

UI_coin::~UI_coin()
{

}

void UI_coin::Load(const nlohmann::json& jsonData)
{
	std::string coinPath = jsonData["coinPath"];
	std::string crossPath = jsonData["crossPath"];
	std::string numberPath = jsonData["numberPath"];

	coinHandle = LoadGraph(coinPath.c_str());
	crossHandle = LoadGraph(crossPath.c_str());
	LoadDivGraph(numberPath.c_str(),
		10, 10, 1, 32, 64, numberHandle);
}

void UI_coin::Initialize()
{
	x = 30;
	y = 700;
	num = "00";
	coinCount = 0;
}

void UI_coin::Update()
{
	num = std::to_string(coinCount);

	//ˆê•¶š‚µ‚©“ü‚Á‚Ä‚È‚¢ê‡æ“ª‚É0‚ğ‘}“ü‚·‚é
	if (num.length() == 1)
	{
		num.insert(0, "0");
	}
}

void UI_coin::Draw()
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