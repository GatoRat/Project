// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

void InitDirectories(std::vector<std::string>& args , const char*    pDocSubdir = nullptr, size_t basePathRemoveCount = 1);
void InitDirectories(std::vector<std::wstring>& args, const wchar_t* pDocSubdir = nullptr, size_t basePathRemoveCount = 1);

const fs::path& GetAppPath();
const fs::path& GetAppBasePath(); 
const fs::path& GetDocPath();
const fs::path& GetDownloadPath();
const fs::path& GetTmpPath();

const fs::path& GetAppDataPath();
const fs::path& GetRoamingDataPath();
const fs::path& GetProgramDataPath();

const fs::path& GetDesktopPath();