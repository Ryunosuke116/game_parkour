#include "common.h"
#include <memory>
#include <string>
#include "UIButton.h"
#include "JsonManager.h"

UIButton::UIButton() :
    titleButtonHandle(-1),
    backGroundHandle(-1)
{
    jsonTag = "png";
}

UIButton::~UIButton()
{
    DeleteGraph(titleButtonHandle);
}

void UIButton::Load(const nlohmann::json& jsonData)
{
    std::unordered_map<std::string, std::string> handlePath;

    for (auto& data : jsonData["UIButton"])
    {
        std::string path = data[0];     //HandlePath
        std::string name = data[1];     //path‚Ì–¼‘O

        handlePath[name] = path;
    }
    titleButtonHandle = LoadGraph(handlePath.at("titleButton").c_str());
    backGroundHandle = LoadGraph(handlePath.at("black").c_str());
}

void UIButton::ResultCreate()
{
    Load(JsonManager::GetInstance().GetJsons(jsonTag));
}

void UIButton::ResultInitialize()
{
    x = 400;
    y = 150;
}

void UIButton::ResultUpdate()
{

}

void UIButton::Draw()
{
    DrawExtendGraph(140, 680, 960, 900, backGroundHandle, TRUE);
    DrawExtendGraph(200, 740, 970, 840, titleButtonHandle, TRUE);
}