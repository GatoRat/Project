// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <string>
#include <istream>

bool CheckUtf8Marker(std::istream& stream, std::string& str);
bool CheckUtf8Marker(const char*& pStr);
bool CheckUtf8Marker(char*& pStr);
