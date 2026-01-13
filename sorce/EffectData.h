#pragma once
#include "Common.h"
#include <vector>
#include <unordered_map>

struct EffectInformation
{
    VECTOR position         = VGet(0.0f, 0.0f, 0.0f);
    int time                = 0;
    int playingEffectHandle = 0;
};

struct EffectData
{
    VECTOR position         = VGet(0.0f, 0.0f, 0.0f);
    int time                = 0;
    int playingEffectHandle = 0;
    int resourceHandle      = 0;
    int playingCount        = 0;            //再生されているエフェクトの数
    std::string tag         = "";
    std::vector<int> playingEffectHandles;
};

// ※エフェクトを発生分描画できるようにしたい
class EffectDatas
{
public:
    EffectDatas(int effectHandle, std::string& effectTag);
    ~EffectDatas();

    void AllStopEffect();
    void PlayEffect();

    const int GetResourceHandle()   const { return resourceHandle; }
    const std::string GetTag()      const { return tag; }
private:
    int resourceHandle          = 0;
    std::string tag             = "";

    int playingCount = 0;            //再生されているエフェクトの数
    std::unordered_map<int, EffectInformation> umPlayingEffectHandles;
};