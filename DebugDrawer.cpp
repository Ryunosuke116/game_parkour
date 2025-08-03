#include "common.h"
#include "DebugDrawer.h"

DebugDrawer& DebugDrawer::Instance()
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

void DebugDrawer::Draw()
{
    if (!EnableDebugDraw) return;

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
        DrawCapsule3D(capsule.top, capsule.bottom, capsule.radius, 16, GetColor(0.0f, 0.0f, 0.0f), capsule.color,FALSE);
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
    capsules.clear();
    texts_int.clear();
    texts_float.clear();
    texts_bool.clear();
    texts_VECTOR.clear();
}