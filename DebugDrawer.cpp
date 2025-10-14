#include "Common.h"
#include "DebugDrawer.h"

DebugDrawer& DebugDrawer::GetInstance()
{
    static DebugDrawer instance;
    return instance;
}

void DebugDrawer::InformationInput_line(const VECTOR& start, const VECTOR& end, unsigned int color)
{
    if (!EnableDebugDraw) return;
    lines.push_back({ start, end, color });
}

void DebugDrawer::InformationInput_sphere(const VECTOR& center, float radius, unsigned int color)
{
    if (!EnableDebugDraw) return;
    spheres.push_back({ center, radius, color });
}

void DebugDrawer::InformationInput_capsule(const VECTOR& top, const VECTOR& bottom, const float radius, unsigned int color)
{
    if (!EnableDebugDraw) return;
    capsules.push_back({ top, bottom, radius, color });
}

void DebugDrawer::InformationInput_string_int(const std::string& text, const int& variable)
{
    if (!EnableDebugDraw) return;
    texts_int.push_back({ text, variable});
}

void DebugDrawer::InformationInput_string_float(const std::string& text, const float& variable)
{
    if (!EnableDebugDraw) return;
    texts_float.push_back({ text, variable });
}

void DebugDrawer::InformationInput_string_bool(const std::string& text, const bool& variable)
{
    if (!EnableDebugDraw) return;
    texts_bool.push_back({ text, variable });
}

void DebugDrawer::InformationInput_string_VECTOR(const std::string& text, const VECTOR& variable)
{
    if (!EnableDebugDraw) return;
    texts_VECTOR.push_back({ text, variable });
}

void DebugDrawer::InformationInput_AABB(const VECTOR& min, const VECTOR& max, unsigned int color)
{
    if (!EnableDebugDraw) return;
    aabbs.push_back({ min,max,color });
}

void DebugDrawer::Draw()
{
    if (!EnableDebugDraw) return;

    frameCount++;

    int nowTime = GetNowCount();
    if (nowTime - startTime >= 1000)  // 1秒経過ごとに計算
    {
        fps = frameCount * 1000.0f / (nowTime - startTime);
        startTime = nowTime;
        frameCount = 0;
    }

    printfDx("FPS : %.1f\n", fps);

    for (const auto& line : lines)
    {
        DrawLine3D(line.start, line.end, line.color);
    }

    for (const auto& sphere : spheres)
    {
        DrawSphere3D(sphere.center, sphere.radius, 16, GetColor(0, 0, 0), sphere.color,FALSE);
    }

    for (const auto& capsule : capsules)
    {
        DrawCapsule3D(capsule.top, capsule.bottom, capsule.radius, 16, GetColor(0, 0, 0), capsule.color,FALSE);
    }

    for (const auto& aabb : aabbs)
    {
        DrawAABB(aabb.min, aabb.max, aabb.color);
    }

    for (const auto& text : texts_int)
    {
        printfDx(text.text.c_str(), text.variable);
    }

    for (const auto& text : texts_float)
    {
        printfDx(text.text.c_str(), text.variable);
    }

    for (const auto& text : texts_bool)
    {
        printfDx(text.text.c_str(), text.variable);
    }

    for (const auto& text : texts_VECTOR)
    {
        printfDx(text.text.c_str(), text.variable.x,
            text.variable.y,
            text.variable.z);
    }

    Clear();
}

void DebugDrawer::Clear()
{
    lines.clear();
    spheres.clear();
    aabbs.clear();
    capsules.clear();
    texts_int.clear();
    texts_float.clear();
    texts_bool.clear();
    texts_VECTOR.clear();
}

void DebugDrawer::DrawAABB(const VECTOR& min, const VECTOR& max, unsigned int color)
{
    VECTOR p[8];

    // 各頂点座標（minとmaxから8点を作る）
    p[0] = VGet(min.x, min.y, min.z);
    p[1] = VGet(max.x, min.y, min.z);
    p[2] = VGet(max.x, max.y, min.z);
    p[3] = VGet(min.x, max.y, min.z);

    p[4] = VGet(min.x, min.y, max.z);
    p[5] = VGet(max.x, min.y, max.z);
    p[6] = VGet(max.x, max.y, max.z);
    p[7] = VGet(min.x, max.y, max.z);

    // 底面
    DrawLine3D(p[0], p[1], color);
    DrawLine3D(p[1], p[2], color);
    DrawLine3D(p[2], p[3], color);
    DrawLine3D(p[3], p[0], color);

    // 上面
    DrawLine3D(p[4], p[5], color);
    DrawLine3D(p[5], p[6], color);
    DrawLine3D(p[6], p[7], color);
    DrawLine3D(p[7], p[4], color);

    // 側面
    DrawLine3D(p[0], p[4], color);
    DrawLine3D(p[1], p[5], color);
    DrawLine3D(p[2], p[6], color);
    DrawLine3D(p[3], p[7], color);
}