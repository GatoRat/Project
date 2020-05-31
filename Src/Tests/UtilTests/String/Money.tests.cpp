// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/String/Money.h"
#include "TestHelper/catch.hpp"

using namespace std;

//   bool ToVal(const char* pStr, int64_t& val, const char** pEnd, char thousandsSeparator = ASCII::Comma, char decimalPoint = ASCII::Period);

TEST_CASE("String/Money::ToVal")
{
    SECTION("nullptr")
    {
        uint64_t val = 0;
        REQUIRE_FALSE(Money::ToVal(nullptr, val)); //-V521
        REQUIRE(val == 0); //-V521
    }
    SECTION("empty")
    {
        const char* pEnd = nullptr;
        uint64_t val = 0;
        REQUIRE_FALSE(Money::ToVal("", val, &pEnd)); //-V521
        REQUIRE(val == 0); //-V521
        REQUIRE(*pEnd == 0); //-V521
    }
    SECTION("100")
    {
        const char* pEnd = nullptr;
        uint64_t val = 0;
        REQUIRE(Money::ToVal("100", val, &pEnd)); //-V521
        REQUIRE(val == 1000000); //-V521
        REQUIRE(*pEnd == 0); //-V521
    }
    SECTION(" 123.45")
    {
        const char* pEnd = nullptr;
        uint64_t val = 0;
        REQUIRE(Money::ToVal(" 123.45", val, &pEnd)); //-V521
        REQUIRE(val == 1234500); //-V521
        REQUIRE(*pEnd == 0); //-V521
    }
    SECTION(" 123.")
    {
        const char* pEnd = nullptr;
        uint64_t val = 0;
        REQUIRE(Money::ToVal(" 123.", val, &pEnd)); //-V521
        REQUIRE(val == 1230000); //-V521
        REQUIRE(*pEnd == 0); //-V521
    }
    SECTION(" 0.45")
    {
        const char* pEnd = nullptr;
        uint64_t val = 0;
        REQUIRE(Money::ToVal(" 0.45", val, &pEnd)); //-V521
        REQUIRE(val == 4500); //-V521
        REQUIRE(*pEnd == 0); //-V521
    }
    SECTION(" .45")
    {
        const char* pEnd = nullptr;
        uint64_t val = 0;
        REQUIRE_FALSE(Money::ToVal(" .45", val, &pEnd)); //-V521
        REQUIRE(val == 0); //-V521
        REQUIRE(*pEnd == '.'); //-V521
    }
    SECTION("-$123.45")
    {
        const char* pEnd = nullptr;
        uint64_t val = 0;
        REQUIRE_FALSE(Money::ToVal("-$123.45", val, &pEnd)); //-V521
        REQUIRE(val == 0); //-V521
        REQUIRE(*pEnd == '-'); //-V521
    }
    SECTION("$(1,234.567)")
    {
        const char* pEnd = nullptr;
        uint64_t val = 0;
        REQUIRE_FALSE(Money::ToVal("$(1,234.567)", val, &pEnd)); //-V521
        REQUIRE(val == 0); //-V521
        REQUIRE(*pEnd == '('); //-V521
    }
    SECTION("(1,234.567)")
    {
        const char* pEnd = nullptr;
        uint64_t val = 0;
        REQUIRE_FALSE(Money::ToVal("(1,234.567)", val, &pEnd)); //-V521
        REQUIRE(val == 0); //-V521
        REQUIRE(*pEnd == '('); //-V521
    }
    SECTION("$10 in cash")
    {
        const char* pEnd = nullptr;
        uint64_t val = 0;
        REQUIRE_FALSE(Money::ToVal("$10 in cash", val, &pEnd)); //-V521
        REQUIRE(val == 100000); //-V521
        REQUIRE(*pEnd != 0); //-V521
    }
}

TEST_CASE("String/Money::ToVal2")
{
    SECTION("nullptr")
    {
        int64_t val = 0;
        REQUIRE_FALSE(Money::ToVal2(nullptr, val)); //-V521
        REQUIRE(val == 0); //-V521
    }
    SECTION("empty")
    {
        const char* pEnd = nullptr;
        int64_t val = 0;
        char currency = 0;
        REQUIRE_FALSE(Money::ToVal2("", val, '.', ',', &currency, &pEnd)); //-V521
        REQUIRE(val == 0); //-V521
        REQUIRE(*pEnd == 0); //-V521
        REQUIRE(currency == 0); //-V521
    }
    SECTION("@123.45")
    {
        const char* pEnd = nullptr;
        int64_t val = 0;
        char currency = 'P';
        REQUIRE(Money::ToVal2("P123.45", val, '.', ',', &currency, &pEnd)); //-V521
        REQUIRE(val == 1234500); //-V521
        REQUIRE(*pEnd == 0); //-V521
        REQUIRE(currency == 'P'); //-V521
    }
    SECTION("@123.45_2")
    {
        const char* pEnd = nullptr;
        int64_t val = 0;
        char currency = 0;
        REQUIRE(Money::ToVal2("P 123.45", val, '.', ',', &currency, &pEnd)); //-V521
        REQUIRE(val == 1234500); //-V521
        REQUIRE(*pEnd == 0); //-V521
        REQUIRE(currency == 'P'); //-V521
    }
    SECTION("100")
    {
        const char* pEnd = nullptr;
        int64_t val = 0;
        char currency = 0;
        REQUIRE(Money::ToVal2("100", val, '.', ',', &currency, &pEnd)); //-V521
        REQUIRE(val == 1000000); //-V521
        REQUIRE(*pEnd == 0); //-V521
        REQUIRE(currency == 0); //-V521
    }
    SECTION(" 123.45")
    {
        const char* pEnd = nullptr;
        int64_t val = 0;
        char currency = 0;
        REQUIRE(Money::ToVal2(" 123.45", val, '.', ',', &currency, &pEnd)); //-V521
        REQUIRE(val == 1234500); //-V521
        REQUIRE(*pEnd == 0); //-V521
        REQUIRE(currency == 0); //-V521
    }
    SECTION(" 123.")
    {
        const char* pEnd = nullptr;
        int64_t val = 0;
        char currency = 0;
        REQUIRE(Money::ToVal2(" 123.", val, '.', ',', &currency, &pEnd)); //-V521
        REQUIRE(val == 1230000); //-V521
        REQUIRE(*pEnd == 0); //-V521
        REQUIRE(currency == 0); //-V521
    }
    SECTION(" 0.45")
    {
        const char* pEnd = nullptr;
        int64_t val = 0;
        char currency = 0; 
        REQUIRE(Money::ToVal2(" 0.45", val, '.', ',', &currency, &pEnd)); //-V521
        REQUIRE(val == 4500); //-V521
        REQUIRE(*pEnd == 0); //-V521
        REQUIRE(currency == 0); //-V521
    }
    SECTION(" .45")
    {
        const char* pEnd = nullptr;
        int64_t val = 0;
        char currency = 0;
        REQUIRE_FALSE(Money::ToVal2(" .45", val, '.', ',', &currency, &pEnd)); //-V521
        REQUIRE(val == 0); //-V521
        REQUIRE(*pEnd == '.'); //-V521
        REQUIRE(currency == 0); //-V521
    }
    SECTION("@123.45")
    {
        const char* pEnd = nullptr;
        int64_t val = 0;
        char currency = '$';
        REQUIRE_FALSE(Money::ToVal2("P123.45", val, '.', ',', &currency, &pEnd)); //-V521
        REQUIRE(val == 0); //-V521
        REQUIRE(*pEnd == 'P'); //-V521
        REQUIRE(currency == '$'); //-V521
    }
    SECTION("-$123.45")
    {
        const char* pEnd = nullptr;
        int64_t val = 0;
        char currency = 0;
        REQUIRE_FALSE(Money::ToVal2("-$123.45", val, '.', ',', &currency, &pEnd)); //-V521
        REQUIRE(val == 0); //-V521
        REQUIRE(*pEnd == '-'); //-V521
        REQUIRE(currency == 0); //-V521
    }
    SECTION("$(1,234.567)")
    {
        const char* pEnd = nullptr;
        int64_t val = 0;
        char currency = 0;
        REQUIRE_FALSE(Money::ToVal2("$(1,234.567)", val, '.', ',', &currency, &pEnd)); //-V521
        REQUIRE(val == 0); //-V521
        REQUIRE(*pEnd == '('); //-V521
        REQUIRE(currency == '$'); //-V521
    }
    SECTION("(1,234.567)")
    {
        const char* pEnd = nullptr;
        int64_t val = 0;
        char currency = 0;
        REQUIRE_FALSE(Money::ToVal2("(1,234.567)", val, '.', ',', &currency, &pEnd)); //-V521
        REQUIRE(val == 0); //-V521
        REQUIRE(*pEnd == '('); //-V521
        REQUIRE(currency == 0); //-V521
    }
    SECTION("$10 in cash")
    {
        const char* pEnd = nullptr;
        int64_t val = 0;
        char currency = 0;
        REQUIRE_FALSE(Money::ToVal2("$10 in cash", val, '.', ',', &currency, &pEnd)); //-V521
        REQUIRE(val == 100000); //-V521
        REQUIRE(*pEnd != 0); //-V521
        REQUIRE(currency == '$'); //-V521
    }
}
