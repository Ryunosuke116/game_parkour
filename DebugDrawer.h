#pragma once
#include <string>
#include <vector>

class DebugDrawer
{
public:
    static DebugDrawer& Instance();  // シングルトン取得

    void InformationInput_line(const VECTOR& start, const VECTOR& end, unsigned int color);
    void InformationInput_sphere(const VECTOR& center, const float radius, unsigned int color);
    void InformationInput_capsule(const VECTOR& top, const VECTOR& bottom, const float radius, unsigned int color);
    void InformationInput_string_int(const std::string& text,  const int& variable );
    void InformationInput_string_float(const std::string& text, const float& variable);
    void InformationInput_string_bool(const std::string& text, const bool& variable);
    void InformationInput_string_VECTOR(const std::string& text, const VECTOR& variable);

    void Clear(); // 描画リストを削除
    void Draw(); // 登録された全てのデバッグ描画を行う

    bool EnableDebugDraw = true; // デバッグ描画ON/OFFフラグ

private:
    struct Line { VECTOR start, end; unsigned int color; };
    struct Sphere { VECTOR center; float radius; unsigned int color; };
    struct Text_int { std::string text; int variable; };
    struct Text_float { std::string text; float variable; };
    struct Text_bool { std::string text; bool variable; };
    struct Text_VECTOR { std::string text; VECTOR variable; };

    struct Capsule { VECTOR top; VECTOR bottom; float radius; unsigned int color; };

    std::vector<Line> lines;
    std::vector<Sphere> spheres;
    std::vector<Text_int> texts_int;
    std::vector<Text_float> texts_float;
    std::vector<Text_bool> texts_bool;
    std::vector<Text_VECTOR> texts_VECTOR;
    std::vector<Capsule> capsules;

    DebugDrawer() = default;
    ~DebugDrawer() = default;

};

