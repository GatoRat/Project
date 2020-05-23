// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cmath>

namespace Almost
{
    // These functions are designed to handle relatively large tolerances,
    // not properly handle true floating point equality.

    constexpr float Tolerance = 0.001f;

    inline
    bool Equal(float val1, float val2, float tolerance = Tolerance) noexcept
    {
        return fabs(val1 - val2) < tolerance;
    }

    inline
    bool Zero(float val, float tolerance = Tolerance) noexcept
    {
        return val > -tolerance && val < tolerance;//Equal(val, 0.0f, tolerance);
    }

    inline
    bool One(float val, float tolerance = Tolerance) noexcept
    {
        return Equal(val, 1.0f, tolerance);
    }

    inline
    bool InRange(float val, float min, float max, float tolerance = Tolerance) noexcept
    {
        return !(val < (min + tolerance) || val >(max - tolerance));
    }

    inline
    float EqualGetSecond(float val, float targetVal, float tolerance = Tolerance) noexcept
    {
        return Equal(val, targetVal, tolerance) ? targetVal : val;
    }

    inline
    float ZeroGetZero(float val, float tolerance = Tolerance) noexcept
    {
        return EqualGetSecond(val, 0.0f, tolerance);
    }
}

constexpr float ForceToRange(float val, float minVal, float maxVal) noexcept
{
    if (val < minVal)
    {
        val = minVal;
    }
    else if (val > maxVal)
    {
        val = maxVal;
    }
    return val;
}
