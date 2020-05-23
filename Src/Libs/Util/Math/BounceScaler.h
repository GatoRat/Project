// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

constexpr float MinPeriod = 0.001f;

float CalcBounceScale(float elapsed, float period, float crestMultiplier, float troughMultiplier);

class BounceScaler
{
public:
    BounceScaler() = default;
    BounceScaler(float period, float crestMultiplier, float troughMultiplier);
    virtual ~BounceScaler() = default;

    void set(float period, float crestMultiplier, float troughMultiplier);

    float calc(float elapsed) const;

public:
    float _period          { 1.0f };
    float _crestMultiplier { 1.0f };
    float _troughMultiplier{ 1.0f };
};
