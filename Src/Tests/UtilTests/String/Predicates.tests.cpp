// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/String/Predicates.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("string/predicates")
{
    SECTION("str_equal_to")
    {
        str_equal_to strEqualTo;
        REQUIRE(strEqualTo.operator()("one", "one") != false); //-V521
        REQUIRE(strEqualTo.operator()("one", "One") == false); //-V521
        REQUIRE(strEqualTo.operator()("one", "on")  == false); //-V521
        REQUIRE(strEqualTo.operator()("one", "one two") == false); //-V521
    }
    SECTION("str_equal_to_nocase")
    {
        str_equal_to_nocase strEqualToNoCase;
        REQUIRE(strEqualToNoCase.operator()("abc", "abd") == false); //-V521
        REQUIRE(strEqualToNoCase.operator()("abc", "abb") == false); //-V521
        REQUIRE(strEqualToNoCase.operator()("ABC", "abc") != false); //-V521
        REQUIRE(strEqualToNoCase.operator()("abc", "ABC") != false); //-V521
        REQUIRE(strEqualToNoCase.operator()("abC", "ABc") != false); //-V521
        REQUIRE(strEqualToNoCase.operator()("abC", "ABd") == false); //-V521
    }
    SECTION("str_less_than")
    {
        str_less_than strLessThan;
        REQUIRE(strLessThan.operator()("abc", "abd") != false); //-V521
        REQUIRE(strLessThan.operator()("abc", "abb") == false); //-V521
        REQUIRE(strLessThan.operator()("ABC", "abc") != false); //-V521
        REQUIRE(strLessThan.operator()("abc", "ABC") == false); //-V521
    }
    SECTION("str_less_than_nocase")
    {
        str_less_than_nocase strLessThanNoCase;
        REQUIRE(strLessThanNoCase.operator()("abc", "abd") != false); //-V521
        REQUIRE(strLessThanNoCase.operator()("abc", "abb") == false); //-V521
        REQUIRE(strLessThanNoCase.operator()("ABC", "abc") == false); //-V521
        REQUIRE(strLessThanNoCase.operator()("abc", "ABC") == false); //-V521
        REQUIRE(strLessThanNoCase.operator()("abC", "ABc") == false); //-V521
        REQUIRE(strLessThanNoCase.operator()("abC", "ABd") != false); //-V521
    }
    SECTION("str_hash_sdbm")
    {
        str_hash_sdbm strHash;
        REQUIRE(strHash.operator()("abc") == 0x3025f862); //-V521
        REQUIRE(strHash.operator()("heather") == 0x46353de5); //-V521
    }
}
