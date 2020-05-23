// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

constexpr float CalcScaleMin = 0.001f;

float CalcScale(float srcWidth, float srcHeight, float targetWidth, float targetHeight);

float CalcSineScale(float elapsed, float period, float amplitude);
