// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <functional>
#include <string_view>

typedef std::function<std::string_view(std::string_view)> JsonStringViewHandler;
