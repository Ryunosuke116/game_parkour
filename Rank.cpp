#include "common.h"
#include <memory>
#include <string>
#include "Rank.h"
#include "JsonManager.h"

Rank::Rank() :
    rankHandle(-1),
    speechBubbleHandle(-1),
    coinCount(-1),
    rankPosX(-1),
    rankPosY(-1),
    rankWidth(-1),
    rankHeight(-1),
    speechBubblePosX(-1),
    speechBubblePosY(-1),
    speechBubbleWidth(-1),
    speechBubbleHeight(-1)
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
    std::unordered_map<std::string, std::string> handlePath;
    std::string speechBubblePath = jsonData["speechBubble"];

    speechBubbleHandle = LoadGraph(speechBubblePath.c_str());

    for (auto& data : jsonData["rankList"])
    {
        std::string path = data[0];     //HandlePath
        std::string name = data[1];     //path‚Ì–¼‘O

        handlePath[name] = path;
    }

    if (coinCount <= 20)
    {
        rankHandle = LoadGraph(handlePath.at("D").c_str());
    }
    else if (coinCount <= 40)
    {
        rankHandle = LoadGraph(handlePath.at("C").c_str());
    }
    else if (coinCount <= 60)
    {
        rankHandle = LoadGraph(handlePath.at("B").c_str());
    }
    else if (coinCount < 100)
    {
        rankHandle = LoadGraph(handlePath.at("A").c_str());
    }
    else if (coinCount >= 100)
    {
        rankHandle = LoadGraph(handlePath.at("S").c_str());
    }
}

void Rank::ResultCreate(const int coinCount)
{
    this->coinCount = 100;
    Load(JsonManager::GetInstance().GetJsons(jsonTag));
}

void Rank::ResultInitialize()
{
    x = 400;
    y = 150;
    rankPosX = 1000;
    rankPosY = 150;
    rankWidth = 300;
    rankHeight = 300;
    speechBubblePosX = 850;
    speechBubblePosY = 0;
    speechBubbleWidth = 600;
    speechBubbleHeight = 600;
    
    rankScale = 0;
    addScaling = 2;
}

void Rank::ResultUpdate()
{
    rankScale += addScaling;

    if (rankScale <= 0 || rankScale >= 50)
    {
        addScaling = -addScaling;
    }
}

void Rank::Draw()
{
    DrawExtendGraph(speechBubblePosX,
        speechBubblePosY, 
        speechBubblePosX + speechBubbleWidth,
        speechBubblePosY + speechBubbleHeight, 
        speechBubbleHandle, TRUE);

    DrawExtendGraph(rankPosX - rankScale,
        rankPosY - rankScale,
        rankPosX + rankWidth + rankScale,
        rankPosY + rankHeight + rankScale,
        rankHandle, TRUE);
}