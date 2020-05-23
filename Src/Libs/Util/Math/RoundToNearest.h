// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "Almost.h"

/**
    Rounds a value to the nearest multiple.
    @param num The value to round.
    @param roundto The target multiple. If \p roundto is zero, this function will return 0.0f.
*/
constexpr float RoundToNearest(float num, float roundto)
{
    return Almost::Zero(roundto) ? 0.0f : round(num / roundto) * roundto;
    //return roundto > -0.001f && roundto < 0.001f ? 0.0f : round(num / roundto) * roundto;
}
