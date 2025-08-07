#include "Dxlib.h"
#include "BlackOut.h"

/// <summary>
/// インストラクタ
/// </summary>
BlackOut::BlackOut()
{
    alpha = 0;
    isLightChange = false;
}

/// <summary>
/// デストラクタ
/// </summary>
BlackOut::~BlackOut()
{

}

/// <summary>
/// 初期化
/// </summary>
void BlackOut::Initialize()
{
    alpha = 0;
    isLightChange = false;
}

/// <summary>
/// 暗転
/// </summary>
void BlackOut::BlackOutUpdate(float addAlpha)
{

    if (!(alpha >= 300))
    {
        alpha += addAlpha;
    }
}

/// <summary>
/// 明転
/// </summary>
void BlackOut::LightChangeUpdate(float addAlpha)
{

    if (!(alpha <= 0))
    {
        alpha -= addAlpha;
    }
}

/// <summary>
/// 描画
/// </summary>
void BlackOut::Draw()
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
    DrawBox(0, 0, 1600, 900, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

