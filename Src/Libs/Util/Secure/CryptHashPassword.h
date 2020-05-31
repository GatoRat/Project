// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <string_view>
#include <vector>

bool CryptHashPassword(std::string_view password, const void* pSalt, int32_t saltLen, void* pHashBuffer, int32_t hashBufferLen, int32_t iterations = 1000);
bool CryptHashPasswordCheck(std::string_view password, const void* pSalt, int32_t saltLen, const void* pHash, int32_t hashLen, int32_t iterations = 1000);

struct CryptHashPassword2 final
{
    static constexpr int32_t SaltLen = 16;
    static constexpr int32_t HashLen = 32;

    uint8_t salt[SaltLen];
    uint8_t hash[HashLen];

    CryptHashPassword2() = default;
    explicit CryptHashPassword2(std::string_view password, int32_t iterations = 1000);
    ~CryptHashPassword2();

    void set(std::string_view password, int32_t iterations = 1000);
};
