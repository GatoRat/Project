// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <string_view>

// For now this assumes something else has called WSAStartup
// #pragma comment (lib, "Ws2_32.lib") was commented out so
// if the code fails to link, WSAStartup needs to be called and
// the library needs to be linked.

uint32_t ResolveIpAddress(const std::string& address);
