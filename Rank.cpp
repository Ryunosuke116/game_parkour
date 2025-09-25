#include "common.h"
#include <memory>
#include <string>
#include "Rank.h"
#include "JsonManager.h"

Rank::Rank():
    rankHandle(-1),
    coinCount(-1)
{
    jsonTag = "png";
}

Rank::~Rank()
{
    DeleteGraph(rankHandle);
}

void Rank::Load(const nlohmann::json& jsonData)
{
    //”wŒi
    std::unordered_map<std::string, std::string> Path;
    std::string speechBubblePath = jsonData["speechBubble"];

    speechBubbleHandle = LoadGraph(speechBubblePath.c_str());

    for (auto& data : jsonData["rankList"])
    {
        std::string path = data[0];     //HandlePath
        std::string name = data[1];     //path‚Ì–¼‘O

        Path[name] = path;
    }

    if (coinCount <= 20)
    {
        rankHandle = LoadGraph(Path.at("D").c_str());
    }
    else if (coinCount <= 40)
    {
        rankHandle = LoadGraph(Path.at("C").c_str());
    }
    else if (coinCount <= 60)
    {
        rankHandle = LoadGraph(Path.at("B").c_str());
    }
    else if (coinCount <= 90)
    {
        rankHandle = LoadGraph(Path.at("A").c_str());
    }
    else if (coinCount == 100)
    {
        rankHandle = LoadGraph(Path.at("S").c_str());
    }
}

void Rank::ResultCreate(const int coinCount)
{
    this->coinCount = coinCount;
    Load(JsonManager::GetInstance().GetJsons(jsonTag));
}

void Rank::ResultInitialize()
{
    x = 400;
    y = 150;
}

void Rank::ResultUpdate()
{

}

void Rank::Draw()
{
   // DrawGraph(x, y, rankHandle, TRUE);
    DrawExtendGraph(850, 0, 1450, 600, speechBubbleHandle, TRUE);
    DrawExtendGraph(1000, y, 1300, 450, rankHandle, TRUE);
}