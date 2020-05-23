// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "TestGlobalSetup.h"

using namespace std;

int main(int argc, char* argv[])
{
    int result = TestGlobalSetup();
    if (result == 0)
    {
        result = Catch::Session().run(argc, argv);

        TestGlobalCleanup(result);
    }

    // If you put a breakpoint here, exit program by resuming
    // so global static objects, especially temp directories,
    // get cleaned up.
    return result;
}
