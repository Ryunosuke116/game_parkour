#pragma once
#include "common.h"
#include <string>

struct EffectData
{
    VECTOR position;
    int time;
    int playingEffectHandle;
    int resourceHandle;
    std::string tag;
};