// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

static constexpr uint32_t DefaultMaxWavFileLen = 10 * 1024 * 1024;

// Throws an runtime_error if file read error
bool VerifyWAV(const fs::path& pathname, bool full = false, uint32_t maxWavFileLen = DefaultMaxWavFileLen);

bool VerifyWAV(const uint8_t* pData, size_t dataLen, bool full = false);
bool VerifyWAV(const std::vector<uint8_t>& data, bool full = false);
