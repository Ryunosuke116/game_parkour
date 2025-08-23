#pragma once

class BlackOut
{
public:
    BlackOut();
    ~BlackOut();

    void Initialize();
    void BlackOutUpdate(float addAlpha);
    void LightChangeUpdate(float addAlpha);
    void Draw();

    void SetAlpha(float setAlpha) { alpha = setAlpha; }

    float GetAlpha() { return alpha; }
private:

    float alpha;
    bool isLightChange;
};

