#include "common.h"
#include <memory>
#include <string>
#include "Result_backGround.h"

Result_backGround::Result_backGround()
{
	jsonTag = "result";
}

Result_backGround::~Result_backGround()
{

}

void Result_backGround::Load(const nlohmann::json& jsonData)
{
	//”wŒi
	std::unordered_map<std::string, std::string> Path;

    for (auto& data : jsonData)
    {
        std::string path = data[0];     //HandlePath
        std::string name = data[1];     //path‚Ì–¼‘O

        Path[name] = path;
    }

    backGroundHandle = LoadGraph(Path.at("backGround").c_str());
}

void Result_backGround::Initialize()
{
    x = 0;
    y = 0;
}

void Result_backGround::Update()
{

}

void Result_backGround::Draw()
{
    DrawGraph(0, 0, backGroundHandle, TRUE);
}