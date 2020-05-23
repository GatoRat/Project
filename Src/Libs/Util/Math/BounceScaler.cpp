// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "BounceScaler.h"
#include <algorithm>
#include <cmath>
#include "Almost.h"
#include "Trig.h"

using namespace std;

float CalcBounceScale(float elapsed, float period, float crestMultiplier, float troughMultiplier)
{
    const float sineValue = sin(elapsed * (RadiansInCircle / (period < MinPeriod ? MinPeriod : period)));
    return 1.0f + ((sineValue > 0.0f ? crestMultiplier : troughMultiplier) * sineValue);
}

BounceScaler::BounceScaler(float period, float crestMultiplier, float troughMultiplier)
    : _period           { period  }
    , _crestMultiplier  { crestMultiplier  }
    , _troughMultiplier { troughMultiplier }
{
}

void BounceScaler::set(float period, float crestMultiplier, float troughMultiplier)
{
    _period           = period;
    _crestMultiplier  = crestMultiplier;
    _troughMultiplier = troughMultiplier;
}

float BounceScaler::calc(float elapsed) const
{
    return CalcBounceScale(elapsed, _period, _crestMultiplier, _troughMultiplier);
}
