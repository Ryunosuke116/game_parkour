#include "common.h"
#include "EffekseerForDXLib.h"
#include "Effect.h"

/// <summary>
/// コンストラクタ
/// </summary>
Effect::Effect()
{
    position = VGet(0, 0, 0);
    time = 0;
    playingEffectHandle = -1;
    resourceHandle = -1;
}

/// <summary>
/// デストラクタ
/// </summary>
Effect::~Effect()
{

}