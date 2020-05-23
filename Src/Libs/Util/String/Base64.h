// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <string>
#include <vector>

std::string Base64Encode(const void* pSrc, size_t srcLen, size_t lineLen = 76, bool useOnlyLF = false);
std::vector<uint8_t> Base64Decode(const char* pSrc);
