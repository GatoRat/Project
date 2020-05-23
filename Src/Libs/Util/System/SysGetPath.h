// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace SysGetPath
{
    fs::path App();
    fs::path Doc();
    fs::path Download();
    fs::path Tmp();

    fs::path BaseAppData();
    fs::path BaseRoamingData();
    fs::path BaseProgramData();

    fs::path AppData();
    fs::path RoamingData();
    fs::path ProgramData();

    fs::path Desktop();
}
