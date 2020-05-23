// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Error/Xception.h"
#include "TestHelper/catch.hpp"
#include <sstream>

using namespace std;

static optional<string> GetErrStrTest(intptr_t code)
{
    return to_string(code);
}

TEST_CASE("Misc/Xception")
{
    SECTION("ctor(intptr_t code, const char* pMsg, GetErrStrTest)")
    {
        const char* pMsg = "One";
        const intptr_t code = 1;

        Xception e(code, pMsg, GetErrStrTest);

        REQUIRE(strcmp(pMsg, e.what()) == 0); //-V521
        REQUIRE(code == e.code()); //-V521
        REQUIRE(e.codeStr() == "1"); //-V521
        REQUIRE(e.hasWhat() != false); //-V521

        stringstream ss;
        ss << e;
        string result(ss.str());
        REQUIRE(result == "1: One"); //-V521
    }
    SECTION("ctor(intptr_t code, const string& msg, GetErrStrTest)")
    {
        string msg("One");
        const intptr_t code = 1;

        Xception e(code, msg, GetErrStrTest);

        REQUIRE(msg == e.what()); //-V521
        REQUIRE(code == e.code()); //-V521
        REQUIRE(e.codeStr() == "1"); //-V521
        REQUIRE(e.hasWhat() != false); //-V521

        stringstream ss;
        ss << e;
        string result(ss.str());
        REQUIRE(result == "1: One"); //-V521
    }
    SECTION("ctor(intptr_t code)")
    {
        const intptr_t code = 1;

        Xception e(code);

        REQUIRE(e.hasWhat() == false); //-V521
        REQUIRE(*e.what() == 0); //-V521
        REQUIRE(code == e.code()); //-V521
        REQUIRE_FALSE(e.codeStr().empty()); //-V521

        stringstream ss;
        ss << e;
        string result(ss.str());
        REQUIRE(result == "1"); //-V521
    }
    SECTION("ctor(intptr_t code, const char* pMsg, nullptr)")
    {
        const char* pMsg = "One";
        const intptr_t code = 1;

        Xception e(code, pMsg, nullptr);

        REQUIRE(strcmp(pMsg, e.what()) == 0); //-V521
        REQUIRE(code == e.code()); //-V521
        REQUIRE_FALSE(e.codeStr().empty()); //-V521
        REQUIRE(e.hasWhat() != false); //-V521

        stringstream ss;
        ss << e;
        string result(ss.str());
        REQUIRE(result == "1: One"); //-V521
    }
    SECTION("ctor(intptr_t code, const string& msg, nullptr)")
    {
        string msg("One");
        const intptr_t code = 1;

        Xception e(code, msg, nullptr);

        REQUIRE(msg == e.what()); //-V521
        REQUIRE(code == e.code()); //-V521
        REQUIRE_FALSE(e.codeStr().empty()); //-V521
        REQUIRE(e.hasWhat() != false); //-V521

        stringstream ss;
        ss << e;
        string result(ss.str());
        REQUIRE(result == "1: One"); //-V521
    }
}