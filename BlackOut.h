#pragma once

class BlackOut 
{
public:
    static BlackOut& GetInstance();


    void Initialize();
    void ResultInitialize();
    void BlackOutUpdate(int kAddAlpha);
    void LightChangeUpdate(int kAddAlpha);
    void Draw();

    void SetAlpha(int setAlpha) { alpha = setAlpha; }
    void SetIsLightChange(const bool flag) { isLightChange = flag; }

    int GetAlpha() { return alpha; }
    bool GetIsLightChange() { return isLightChange; }
private:

    int alpha;
    bool isLightChange;
    BlackOut() = default;
    ~BlackOut() = default;

    static constexpr int kMaxAlpha = 300;
};