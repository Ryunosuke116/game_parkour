#pragma once
#include "Common.h"
#include <vector>
#include <unordered_map>

struct EffectInformation
{
    VECTOR position         = VGet(0.0f, 0.0f, 0.0f);
    int time                = 0;
    int playingCount        = 0;
    int resourceHandle      = 0;
    std::string tag         = "";
    std::vector<int> playingEffectHandle;
    std::unordered_map<int, int> umPlayingEffectHandles;
};

struct EffectData
{
    VECTOR position         = VGet(0.0f, 0.0f, 0.0f);
    int time                = 0;
    int playingEffectHandle = 0;
    int resourceHandle      = 0;
    std::string tag         = "";
};

// ※エフェクトを発生分描画できるようにしたい
class EffectDatas
{
public:
    

private:
    int resourceHandle          = 0;
    std::string tag             = "";
    EffectInformation effectInformation;
};