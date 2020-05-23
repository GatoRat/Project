// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Misc/AppInfo.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Misc/AppInfo")
{
    AppInfo::SetVersion(1, 2, 3, 4); //-V112

    AppInfo::Version version = AppInfo::GetVersion();

    REQUIRE(version.major == 1); //-V112 //-V521
    REQUIRE(version.minor == 2); //-V112 //-V521
    REQUIRE(version.build == 3); //-V112 //-V521
    REQUIRE(version.revision == 4); //-V112 //-V521
    REQUIRE(AppInfo::GetVersionStr() == "1.2.3.4"); //-V521
    REQUIRE(AppInfo::GetName() == "UtilTests"); //-V521

    fs::path exeName = AppInfo::GetExeName();

    REQUIRE_FALSE(exeName.empty()); //-V521
    REQUIRE(exeName.string() == "UtilTests"); //-V521

    fs::path exePath = AppInfo::GetExePath();

    REQUIRE_FALSE(exePath.empty()); //-V521
    REQUIRE_FALSE(exePath.root_path().empty()); //-V521
}
