#pragma once
#include <string>
#include <vector>

class DebugDrawer
{
public:
    static DebugDrawer& GetInstance();  // シングルトン取得

    void InformationInputLine(const VECTOR& start, const VECTOR& end, unsigned int color);
    void InformationInputSphere(const VECTOR& center, const float radius, unsigned int color);
    void InformationInputAABB(const VECTOR& min, const VECTOR& max, unsigned int color);
    void InformationInputCapsule(const VECTOR& top, const VECTOR& bottom, const float radius, unsigned int color);
    void InformationInputStringInt(const std::string& text,  const int& variable );
    void InformationInputStringFloat(const std::string& text, const float& variable);
    void InformationInputStringBool(const std::string& text, const bool& variable);
    void InformationInputStringVector(const std::string& text, const VECTOR& variable);
    void DrawAABB(const VECTOR& min, const VECTOR& max, unsigned int color);
    void Clear(); // 描画リストを削除
    void Draw(); // 登録された全てのデバッグ描画を行う

    // Release
#if defined(NDEBUG)
    bool EnableDebugDraw = false; // デバッグ描画ON/OFFフラグ
#else
    //　Debug
    bool EnableDebugDraw = true; // デバッグ描画ON/OFFフラグ
#endif

private:
    struct Line { VECTOR start, end; unsigned int color; };
    struct Sphere { VECTOR center; float radius; unsigned int color; };
    struct AABB { VECTOR min; VECTOR max; unsigned int color; };
    struct Capsule { VECTOR top; VECTOR bottom; float radius; unsigned int color; };
    struct Text_int { std::string text; int variable; };
    struct Text_float { std::string text; float variable; };
    struct Text_bool { std::string text; bool variable; };
    struct Text_VECTOR { std::string text; VECTOR variable; };

    std::vector<Line> lines;
    std::vector<Sphere> spheres;
    std::vector<AABB> aabbs;
    std::vector<Capsule> capsules;
    std::vector<Text_int> texts_int;
    std::vector<Text_float> texts_float;
    std::vector<Text_bool> texts_bool;
    std::vector<Text_VECTOR> texts_VECTOR;

    int frameCount = 0;
    int startTime = 0;
    float fps = 0.0f;

    DebugDrawer() = default;
    ~DebugDrawer() = default;

};

