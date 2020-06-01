// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Misc/TimedTest.h"
#include "Util/Error/Xception.h"
#include "Util/String/Convert.h"
#include "Util/String/Str.h"
#include <iostream>

using namespace std;

static uint32_t numTests = 1;
static uint32_t loopsPerTest = 1;

void Usage()
{
    cout << "testTimed [numTests [loopsPerTest]]" << endl;
}

void ParseCommandLine(int argc, char* argv[])
{
    int nonSwitchArg = 0;

    for (int arg = 1; arg < argc; ++arg)
    {
        if (argv[arg][0] == '-' || argv[arg][0] == '/')
        {
            if (argv[arg][1] == '?' ||
                Str::Compare(&argv[arg][1], "help", true) == 0)
            {
                Usage();
                throw Xception(1);
            }
            else
            {
                cout << "Unknown switch: " << argv[arg] << endl;
                throw Xception(1);
            }
        }
        else
        {
            switch (nonSwitchArg++)
            {
                case 0:
                    if (!Convert::ToUint32(argv[arg], numTests))
                    {
                        cout << "Invalid value for numTests (" << argv[arg] << ")" << endl;
                        throw Xception(1);
                    }
                    break;

                case 1:
                    if (!Convert::ToUint32(argv[arg], loopsPerTest))
                    {
                        cout << "Invalid value for loopsPerTest (" << argv[arg] << ")" << endl;
                        throw Xception(1);
                    }
                    break;

                default:
                    cout << "Unknown arg: " << argv[arg] << endl;
                    throw Xception(1);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    int result = 1;
    try
    {
        ParseCommandLine(argc, argv);
        RunTimedTests(numTests, loopsPerTest, false);
        result = 0;
    }
    catch (const Xception& e)
    { //-V565

        cerr << e << endl;
    }
    return result;
}
