// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

constexpr float PI              = 3.14159265358979323846f;
constexpr float RadiansIn90     = PI / 2.0f;
constexpr float RadiansIn180    = PI;
constexpr float RadiansIn360    = PI * 2.0f;
constexpr float RadiansInCircle = RadiansIn360;

constexpr float DegreesInCircle  = 360.0f;

constexpr float DegreesToRadians(float degrees)
{
    return degrees * (PI / 180.0f);
}

constexpr float RadiansToDegrees(float radians)
{
    return (radians * 180.0f) / PI;
}
