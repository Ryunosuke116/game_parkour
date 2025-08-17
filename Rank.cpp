#include "common.h"
#include <memory>
#include <string>
#include "Rank.h"

Rank::Rank()
{
    jsonTag = "rank";
}

Rank::~Rank()
{

}

void Rank::Load(const nlohmann::json& jsonData)
{
    //”wŒi
    std::unordered_map<std::string, std::string> Path;

    for (auto& data : jsonData)
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

void Rank::Initialize()
{
    x = 400;
    y = 150;
}

void Rank::Update()
{

}

void Rank::Draw()
{
    DrawGraph(x, y, rankHandle, TRUE);
}