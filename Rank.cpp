#include "common.h"
#include <memory>
#include <string>
#include "Rank.h"
#include "JsonManager.h"
#include "Calculation.h"

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
    const int kMaxRankD = 20;
    const int kMaxRankC = 40;
    const int kMaxRankB = 60;
    const int kMaxRankA = 100;

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

    if (coinCount <= kMaxRankD)
    {
        rankHandle = LoadGraph(handlePath.at("D").c_str());
    }
    else if (coinCount <= kMaxRankC)
    {
        rankHandle = LoadGraph(handlePath.at("C").c_str());
    }
    else if (coinCount <= kMaxRankB)
    {
        rankHandle = LoadGraph(handlePath.at("B").c_str());
    }
    else if (coinCount < kMaxRankA)
    {
        rankHandle = LoadGraph(handlePath.at("A").c_str());
    }
    else if (coinCount >= kMaxRankA)
    {
        rankHandle = LoadGraph(handlePath.at("S").c_str());
    }
}

void Rank::ResultCreate(const int coinCount)
{
    this->coinCount = coinCount;
    Load(JsonManager::GetInstance().GetJsons(jsonTag));
}

void Rank::ResultInitialize()
{
    const float kInitRankPosX = 1840.0f;
    const float kInitRankPosY = 150.0f;
    const float lkInitRankWidth = 300.0f;
    const float lkInitRankHeight = 300.0f;
    const float kInitSpeechBubblePosX = 1690.0f;
    const float kInitSpeechBubblePosY = 0.0f;
    const float kInitSpeechBubbleWidth = 600.0f;
    const float kInitSpeechBubbleHeight = 600.0f;
    const int   kInitRankScale = 0;
    const int   kInitAddScaling = 2;

    rankPosX = kInitRankPosX;
    rankPosY = kInitRankPosY;
    rankWidth = lkInitRankWidth;
    rankHeight = lkInitRankHeight;
    speechBubblePosX = kInitSpeechBubblePosX;
    speechBubblePosY = kInitSpeechBubblePosY;
    speechBubbleWidth = kInitSpeechBubbleWidth;
    speechBubbleHeight = kInitSpeechBubbleHeight;
    
    rankScale = kInitRankScale;
    addScaling = kInitAddScaling;
}

void Rank::ResultUpdate()
{
    const int kMaxRankScale = 50;
    const float kTargetrankPosX = 1000.0f;
    const float kTargetspeechBubblePosX = 850.0f;
    const float kLeapSpeed = 0.1f;

    rankScale += addScaling;

    if (rankScale <= 0 || rankScale >= kMaxRankScale)
    {
        addScaling = -addScaling;
    }

    rankPosX = Calculation::Leap(rankPosX, kTargetrankPosX, kLeapSpeed);
    speechBubblePosX = Calculation::Leap(speechBubblePosX, kTargetspeechBubblePosX, kLeapSpeed);
}

void Rank::Draw()
{
    DrawExtendGraphF(speechBubblePosX,
        speechBubblePosY, 
        speechBubblePosX + speechBubbleWidth,
        speechBubblePosY + speechBubbleHeight, 
        speechBubbleHandle, TRUE);

    DrawExtendGraphF(rankPosX - rankScale,
        rankPosY - rankScale,
        rankPosX + rankWidth + rankScale,
        rankPosY + rankHeight + rankScale,
        rankHandle, TRUE);
}