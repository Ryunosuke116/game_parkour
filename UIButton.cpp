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
    const int kInitAlpha = 0;
    const int kInitAddAlpha = 2;

    alpha = kInitAlpha;
    kAddAlpha = kInitAddAlpha;
}

void UIButton::ResultUpdate()
{
    const int kMaxAlpha = 255;

    alpha += kAddAlpha;

    if (alpha <= 0 || alpha >= kMaxAlpha)
    {
        kAddAlpha = -kAddAlpha;
    }
}

void UIButton::Draw()
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
    DrawExtendGraph(140, 680, 960, 900, backGroundHandle, TRUE);
    DrawExtendGraph(200, 740, 970, 840, titleButtonHandle, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}