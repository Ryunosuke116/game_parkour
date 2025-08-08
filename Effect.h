#pragma once
class Effect
{
public:
    Effect();
    ~Effect();

private:
    VECTOR position;
    int time;
    int playingEffectHandle;
    int resourceHandle;

};

