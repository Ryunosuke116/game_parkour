#include "Common.h"
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
    const float kInitBackGroundWidth = 820.0f;
    const float kInitBackGroundHeight = 220.0f;
    const float kInitTitleButtonWidth = 770.0f;
    const float kInitTitleButtonHeight = 100.0f;

    backGroundX = kInitBackGroundX;
    backGroundY = kInitBackGroundY;
    backGroundWidth = kInitBackGroundWidth;
    backGroundHeight = kInitBackGroundHeight;
    titleButtonX = kInitTitleButtonX;
    titleButtonY = kInitTitleButtonY;
    titleButtonWidth = kInitTitleButtonWidth;
    titleButtonHeight = kInitTitleButtonHeight;

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
    DrawExtendGraphF(backGroundX,
        backGroundY, 
        backGroundX + backGroundWidth,
        backGroundY + backGroundHeight,
        backGroundHandle, TRUE);
    DrawExtendGraphF(titleButtonX,
        titleButtonY, 
        titleButtonX + titleButtonWidth,
        titleButtonY + titleButtonHeight,
        titleButtonHandle, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}