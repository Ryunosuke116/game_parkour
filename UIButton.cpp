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
    const float kInitBackGroundX = 140.0f;
    const float kInitBackGroundY = 680.0f;
    const float kInitTitleButtonX = 200.0f;
    const float kInitTitleButtonY = 740.0f;

    backGroundX = kInitBackGroundX;
    backGroundY = kInitBackGroundY;
    titleButtonX = kInitTitleButtonX;
    titleButtonY = kInitTitleButtonY;

    alpha = kInitAlpha;
    addAlpha = kInitAddAlpha;
}

void UIButton::ResultUpdate()
{
    const int kMaxAlpha = 255;
    const int kMinAlpha = 0;

    alpha += addAlpha;

    if (alpha <= kMinAlpha || alpha >= kMaxAlpha)
    {
        addAlpha = -addAlpha;
    }
}

void UIButton::Draw()
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
    DrawExtendGraph(backGroundX, backGroundY, 960, 900, backGroundHandle, TRUE);
    DrawExtendGraph(200, 740, 970, 840, titleButtonHandle, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}