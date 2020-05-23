// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>
#include "Util/Misc/AppInfo.h"
#include "Util/Misc/CommandLine.h"
#include "UtilExp.h"
#include "Version.h"

using namespace std;

int wmain(int argc, wchar_t* argv[])
{
    AppInfo::SetVersion(VER_MAJOR, VER_MINOR, VER_BUILD, VER_REVISION);
    CommandLine::WriteNotice(VER_ASCIICOPYRIGHT_STR);

    int result = 0;

    try
    {
        CommandLine::ArgsW args;
        args.parse(argc, argv);
        result = SummitExp(args._args);
    }
    catch (XceptionT<CommandLine::Error>&)
    { //-V565
    }
    catch (Xception& e)
    {
        if (e.code())
        {
            cout << "ERROR: Unhandled Xception:" << e << endl;
        }
    }

    return ShowExitCode(result);
}
