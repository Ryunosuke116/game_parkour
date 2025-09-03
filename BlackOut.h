#pragma once

class BlackOut 
{
public:
    static BlackOut& GetInstance();


    void Initialize();
    void BlackOutUpdate(int addAlpha);
    void LightChangeUpdate(int addAlpha);
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
};