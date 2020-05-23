// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <optional>
#include <string>

std::string GetDefaultErrStr(intptr_t code);

// This ALWAYS returns a valid string.
std::optional<std::string> GetErrStr(intptr_t code);
std::optional<std::string> GetErrStrErrno(intptr_t code);
std::optional<std::string> GetErrStrOS(intptr_t code);

#ifdef _WIN32
    std::optional<std::wstring> GetErrStrOSW(intptr_t code, const wchar_t* pDefault = nullptr);
#endif