#pragma once
#include "Common.h"

struct EffectData
{
    VECTOR position = VGet(0.0f, 0.0f, 0.0f);
    int time = 0;
    int playingEffectHandle = 0;
    int resourceHandle = 0;
    std::string tag = "";
};