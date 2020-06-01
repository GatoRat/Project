// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CommandLine.h"
#include <iostream>
#include "../String/Str.h"
#include "AppInfo.h"

using namespace std;

static bool pauseBeforeExit = false;
static bool verboseOutput   = true;

namespace CommandLine
{
    void Args::parseArg(unsigned /*argNum*/, const char* pArg)
    {
        args.push_back(pArg);
    }

    const char* Args::getUsage(UsageType /*usageType*/)
    {
        return "";
    }

    bool Args::verify()
    {
        return true;
    }

    void Args::printUsage()
    {
        cout << "Usage: " << AppInfo::GetName() << ' '
             << getUsage(UsageType::Args) << " [-p[ause]] [-v[-]] [-?|-h]\n"
             << getUsage(UsageType::Detail)
             << " -p[ause] : pause before exit.\n"
                " -v : verbose output.\n"
                " -v- : non-verbose output.\n"
                " -? : show this help\n"
             << endl;
    }

    void Args::parse(int argc, char* argv[])
    {
        unsigned argNum = 0;
        for (int arg = 1; arg < argc; arg++)
        {
            if (IsDashOrSlash(argv[arg][0]) && argv[arg][1] == 0)
            {
                cout << "Command Line Error: Invalid switch: " << argv[arg] << endl;
                throw XceptionT<Error>(Error::BadSwitch);
            }

            if (strcmp(argv[arg], "-?") == 0 ||
                strcmp(argv[arg], "-h") == 0 ||
                strcmp(argv[arg], "-help") == 0)
            {
                printUsage();
                throw XceptionT<Error>(Error::Success);
            }
            else if (strcmp(argv[arg], "-p") == 0 ||
                    strcmp(argv[arg], "-pause") == 0)
            {
                pauseBeforeExit = true;
            }
            else if (strcmp(argv[arg], "-p-") == 0 ||
                     strcmp(argv[arg], "-pause-") == 0)
            {
                pauseBeforeExit = false;
            }
            else if (strcmp(argv[arg], "-v") == 0)
            {
                verboseOutput = true;
            }
            else if (strcmp(argv[arg], "-v-") == 0)
            {
                verboseOutput = false;
            }
            else
            {
                parseArg(argNum++, argv[arg]);
            }
        }
    }

    void ArgsW::parseArg(unsigned /*argNum*/, const wchar_t* pArg)
    {
        _args.push_back(pArg);
    }

    const wchar_t* ArgsW::getUsageArgs() const
    {
        return L"";
    }

    const wchar_t* ArgsW::getUsageDetail() const
    {
        return L"";
    }

    const wchar_t* ArgsW::getUsageExtra() const
    {
        return L"";
    }

    bool ArgsW::verify()
    {
        return true;
    }

    void ArgsW::printUsage() const
    {
        wcout << "Usage: " << AppInfo::GetName().c_str() << ' '
              << getUsageArgs();

        if (_options & Options::AllowPauseArg)
        {
            wcout << " [-p[ause]]";
        }

        if (_options & Options::AllowVerboseArg)
        {
            wcout << " [-v[-]]";
        }

        wcout << " [-?|-h]\n\n"
              << getUsageDetail();

        if (_options & Options::AllowPauseArg)
        {
            wcout << " -p[ause]   Pause before exit.\n";
        }

        if (_options & Options::AllowVerboseArg)
        {
            wcout << " -v         Show verbose output.\n"
                  << " -v-        Don't show verbose output.\n";
        }

        wcout << " -?         Show this help\n"
              << getUsageExtra()
              << endl;
    }

    void ArgsW::parse(int argc, wchar_t* argv[], Options options)
    {
        _options = options;

        if ((_options & Options::UsageOnNoArgs) && argc < 2)
        {
            printUsage();
            throw XceptionT<Error>(Error::Success);
        }

        for (int arg = 1; arg < argc; arg++)
        {
            if (wcscmp(argv[arg], L"-?") == 0 ||
                wcscmp(argv[arg], L"/?") == 0 ||
                wcscmp(argv[arg], L"-h") == 0 ||
                wcscmp(argv[arg], L"/h") == 0 ||
                wcscmp(argv[arg], L"-help") == 0 ||
                wcscmp(argv[arg], L"/help") == 0)
            {
                printUsage();
                throw XceptionT<Error>(Error::Success);
            }
        }

        unsigned argNum = 0;
        for (int arg = 1; arg < argc; arg++)
        {
            try
            {
                if (IsDashOrSlash(argv[arg][0]) && argv[arg][1] == 0)
                {
                    throw XceptionT<Error>(Error::BadSwitch);
                }

                if ((_options & Options::AllowPauseArg) &&
                    (wcscmp(argv[arg], L"-p") == 0 || wcscmp(argv[arg], L"-pause") == 0 ||
                     wcscmp(argv[arg], L"/p") == 0 || wcscmp(argv[arg], L"/pause") == 0))
                {
                    pauseBeforeExit = true;
                }
                else if ((_options & Options::AllowPauseArg) &&
                         (wcscmp(argv[arg], L"-p-") == 0 || wcscmp(argv[arg], L"-pause-") == 0 ||
                          wcscmp(argv[arg], L"/p-") == 0 || wcscmp(argv[arg], L"/pause-") == 0))
                {
                    pauseBeforeExit = false;
                }
                else if ((_options & Options::AllowVerboseArg) && (wcscmp(argv[arg], L"-v") == 0 ||
                                                                   wcscmp(argv[arg], L"/v") == 0))
                {
                    verboseOutput = true;
                }
                else if ((_options & Options::AllowVerboseArg) && (wcscmp(argv[arg], L"-v-") == 0 ||
                                                                   wcscmp(argv[arg], L"/v-") == 0))
                {
                    verboseOutput = false;
                }
                else
                {
                    parseArg(argNum++, argv[arg]);
                }
            }
            catch (const XceptionT<Error>& e)
            {
                switch (e.code())
                {
                    case Error::InvalidArg:
                        wcout << L"Command Line Error: Invalid argument: " << argv[arg] << endl;
                        break;

                    case Error::BadSwitch:
                        wcout << L"Command Line Error: Invalid switch: " << argv[arg] << endl;
                        break;
                }
                throw;
            }
        }

        if (!verify())
        {
            printUsage();
            throw XceptionT<Error>(Error::Success);
        }
    }

    void WriteNotice(const char* pCopyright)
    {
        cout << '\n' << AppInfo::GetName() << ' ' << AppInfo::GetVersionStr()
             << ' ' << (pCopyright ? pCopyright : " ") << '\n' << endl;
    }

} // namespace CommandLine

bool VerboseOutput()
{
    return verboseOutput;
}

int ShowExitCode(int exitCode, bool showExitCode)
{
    if (showExitCode && exitCode)
    {
        cout << "\nExit code: " << exitCode << endl;
    }

    if (pauseBeforeExit)
    {
        cout << "\nPress Enter to exit: " << flush;
        cin.get();
    }

    return exitCode;
}
