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

/// <summary>
/// à√ì]
/// </summary>
void BlackOut::BlackOutUpdate(int addAlpha)
{
    if (!(alpha >= 300))
    {
        alpha += addAlpha;
    }
}

/// <summary>
/// ñæì]
/// </summary>
void BlackOut::LightChangeUpdate(int addAlpha)
{
    if (!(alpha <= 0))
    {
        alpha -= addAlpha;
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

