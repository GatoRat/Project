// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <string>

struct OsError
{
    uint32_t code;
    std::string str;

    OsError();
    explicit OsError(uint32_t errCode);
    OsError(uint32_t errCode, const std::string& errStr);

    const std::string& setStr(uint32_t errCode);

    static uint32_t getCode();
};
