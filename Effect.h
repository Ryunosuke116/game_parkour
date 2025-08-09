#pragma once
class Effect
{
public:
    Effect();
    ~Effect();

    void Load(const char* path, std::string& setTag,
        const float& scale, const VECTOR& setPos);
    void PositionUpdate(const VECTOR& setPos);
    void PlayEffect();
    void StopEffect();
    void SetScale(VECTOR scale);
    void SetSpeed(float speed);


private:
    VECTOR position;
    int time;
    int playingEffectHandle;
    int resourceHandle;
    std::string tag;

};

