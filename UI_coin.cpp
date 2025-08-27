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
		-1
	);

	jsonTag = "coin";
}

UI_coin::~UI_coin()
{

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
	cross_x = x + 105;
	cross_y = y + 30;
	number_x = x + 140;
	number_y = y + 30;

	countNumber = "00";
	coinCount = 0;
}

void UI_coin::Update(ObjectMediator& objectMediator)
{
	countNumber = std::to_string(coinCount);

	//ˆê•¶š‚µ‚©“ü‚Á‚Ä‚È‚¢ê‡æ“ª‚É0‚ğ‘}“ü‚·‚é
	if (countNumber.length() == 1)
	{
		countNumber.insert(0, "0");
	}
}

void UI_coin::Draw()
{
	DrawGraph(x, y, coinHandle, TRUE);

	int num_x = number_x;
	const int addNumber_x = 32;

	for (char c : countNumber)
	{
		int digit = c - '0';
		DrawGraph(num_x, number_y, numberHandle[digit], TRUE);
		num_x += addNumber_x;
	}

	DrawGraph(cross_x, cross_y, crossHandle, TRUE);
}