#include "Dxlib.h"
#include "BlackOut.h"


BlackOut& BlackOut::GetInstance()
{
    static BlackOut instance;
    return instance;
}

/// <summary>
/// èâä˙âª
/// </summary>
void BlackOut::Initialize()
{
    alpha = 0;
    isLightChange = false;
}

void BlackOut::ResultInitialize()
{
    alpha = kMaxAlpha;
    isLightChange = true;
}

/// <summary>
/// à√ì]
/// </summary>
void BlackOut::BlackOutUpdate(int kAddAlpha)
{
    if (alpha <= kMaxAlpha)
    {
        alpha += kAddAlpha;
    }
}

/// <summary>
/// ñæì]
/// </summary>
void BlackOut::LightChangeUpdate(int kAddAlpha)
{
    if (alpha > 0)
    {
        alpha -= kAddAlpha;
        if (alpha < 0)
        {
            alpha = 0;
        }
    }
}

/// <summary>
/// ï`âÊ
/// </summary>
void BlackOut::Draw()
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
    DrawBox(0, 0, 1600, 900, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

