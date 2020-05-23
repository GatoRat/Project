// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>

namespace fs = std::filesystem;

// Throws a runtime_exception if file error
bool VerifyPNG(const fs::path& pathname, bool quick = true);
