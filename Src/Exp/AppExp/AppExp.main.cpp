// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "AppExp.h"
#include "Util/Misc/AppInfo.h"
#include "Util/Time/Stopwatch.h"
#include "AppExpVersion.h"

QTextStream output(stdout);
QTextStream errOut(stderr);

static bool pauseBeforeExit = false;

static int ShowExitCode(int exitCode, bool showExitCode = true)
{
    if (showExitCode && exitCode > 0)
    {
        output << endl << QStringLiteral("Exit code: ") << showExitCode << endl;
    }

    if (pauseBeforeExit)
    {
        output << endl << QStringLiteral("Press Enter to exit: ") << flush;
        QTextStream in(stdin);
        in.readLine();
    }

    return exitCode;
}

static int Usage(int exitCode = 0)
{
    output  << QStringLiteral("AppExp [-?|-h] [-d<folder>] [-p] [-l[#]] [-t|-T] [test args]") << endl //-V119
            << QStringLiteral("?  = show this help") << endl
            << QStringLiteral("p  = pause before exit.") << endl
            << QStringLiteral("n# = run test for specified number of loops.") << endl
            << QStringLiteral("t  = time test(s) timer showing seconds.") << endl
            << QStringLiteral("T  = time test(s) timer showing nano-seconds.") << endl
            << TestUsage()
            << endl;

    return ShowExitCode(exitCode, false);
}

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);

    AppInfo::SetVersion(VER_MAJOR, VER_MINOR, VER_BUILD, VER_REVISION);
    application.setApplicationVersion(VER_VERSION_STR);
    application.setOrganizationDomain(VER_COMPANYDOMAIN_STR);
    application.setOrganizationName(VER_COMPANYNAME_STR);

    QStringList arguments = QCoreApplication::arguments();
    if (!InitDirectories(arguments))
    {
        errOut << QStringLiteral("Fatal Error: Required data directories could not be found or created.") << endl;
        return 1;
    }

    //PreferencesInit preferencesInit;
    //LogOpen(5);

    int numLoops = 1;
    bool doTiming = false;
    bool showSeconds = true;

    Arguments args(arguments);

    QStringList remainingArgs;
    remainingArgs += arguments[0];

    while (args.Next())
    {
        switch (args.GetSwitch())
        {
            case 'n':
                if (!args.GetValue(numLoops, 1, true))
                {
                    errOut << QStringLiteral("Error: -n has invalid value: ") << args.GetCurArg() << endl << endl;
                    return ShowExitCode(-1, false);
                }
                if (numLoops < 1)
                {
                    errOut << QStringLiteral("Error: -n has invalid value: ") << numLoops << endl << endl;
                    return ShowExitCode(-1, false);
                }
                break;

            case 'p':
                pauseBeforeExit = args.GetBool(true);
                break;

            case 't':
                doTiming = true;
                showSeconds = true;
                break;

            case 'T':
                doTiming = true;
                showSeconds = false;
                break;

            default:
            case 0:
                remainingArgs += args.GetCurArg();
                break;

            case '?':
            case 'h':
                return Usage();
        }
    }

    Stopwatch totalTimer;
    Stopwatch timer;
    int exitCode = 0;

    for (int i = 0; i < numLoops; i++)
    {
        timer.start();

        exitCode = AppExp(remainingArgs);
        if (exitCode < 0)
        {
            return Usage(exitCode);
        }

        timer.end();
    }

    if (doTiming)
    {
        totalTimer.end();
        totalTimer.calculate();
        timer.calculate();
        if (showSeconds)
        {
            errOut/*output*/ << QStringLiteral("Seconds: ") << totalTimer.getElapsedSeconds() <<  //-V119
                      QStringLiteral(" / ") << timer.getElapsedSeconds() << endl;
        }
        else
        {
            errOut/*output*/ << QStringLiteral("Nanoseconds: ") << totalTimer.getElapsedSeconds() <<  //-V119
                      QStringLiteral(" / ") << timer.getElapsedSeconds() << endl;
        }
    }

    return ShowExitCode(exitCode, true);
}

