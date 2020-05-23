// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <string_view>

bool CryptHashPassword_OpenSSL(std::string_view password, const void* pSalt, int32_t saltLen, void* pHashBuffer, int32_t hashBufferLen, int32_t iterations = 1000);
