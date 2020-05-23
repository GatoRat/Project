// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <string>

void SplitText(std::string_view text, std::string& line1, std::string& line2);

void SplitText(std::string_view text, std::string_view& line1, std::string_view& line2);