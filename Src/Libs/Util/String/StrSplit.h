// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <functional>
#include <string_view>
#include <vector>

/// <summary>
/// String functions.
/// </summary>
namespace Str
{
    std::vector<std::string_view> Split(std::string_view str, char delimiter);

    typedef std::function<bool(size_t num, std::string_view str)> SplitCallback;
    size_t Split(std::string_view str, char delimiter, const SplitCallback& callback);

    typedef std::function<bool(size_t num, char* pStr)> SplitCallback2;
    size_t Split2(char* pStr, char delimiter, const SplitCallback2& callback);

} // namespace Str
