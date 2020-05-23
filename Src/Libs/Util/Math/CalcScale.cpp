// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CalcScale.h"
#include <algorithm>
#include <cmath>
#include "Trig.h"

using namespace std;

float CalcScale(float srcWidth, float srcHeight, float targetWidth, float targetHeight)
{
    float scale = 1.0f;

    if (srcWidth >= CalcScaleMin && srcHeight >= CalcScaleMin)
    {
        if (targetWidth >= CalcScaleMin && targetHeight >= CalcScaleMin)
        {
            scale = min(targetWidth / srcWidth, targetHeight / srcHeight);
            if (scale < CalcScaleMin)
            {
                scale = CalcScaleMin;
            }
        }
    }
    else
    {
        scale = 0.0f;
    }

    return scale;
}

float CalcSineScale(float elapsed, float period, float amplitude)
{
    const float sineValue = sin(elapsed * (RadiansInCircle / (period < CalcScaleMin ? CalcScaleMin : period)));
    return 1.0f + (amplitude * sineValue);
}
