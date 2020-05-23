// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <vector>

std::vector<uint8_t> CryptSha(const void* pData, size_t dataLen, bool use512 = true);
bool CryptSha(const void* pData, size_t dataLen, uint8_t* pHash, size_t hashLen);