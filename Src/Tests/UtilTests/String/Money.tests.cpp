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

TEST_CASE("String/Money/Optima")
{
    SECTION("parseCurrency")
    {
        struct
        {
            const char* pStr;
            struct
            {
                string_view currency;
                const char* pResult;

            } expected;
        }
        data[]
        {
            { "'1234567'" , { "12345", ""   } },
            { nullptr     , { "$" , nullptr } },
            { ""          , { "$" , ""      } },
            { "'"         , { "$" , ""      } },
            { "''"        , { "$" , ""      } },
            { "'P'"       , { "P" , ""      } },
            { "'P',"      , { "P" , ","     } },
            { "'P"        , { "P" , ""      } },
            { "' E'"      , { " E", ""      } },
            { "'R$'."     , { "R$", "."     } },
        };

        for (auto& item : data)
        {
            auto pResult = Money::Optima::parseCurrency(item.pStr);
            if (pResult && item.expected.pResult)
            {
                REQUIRE(string_view(pResult) == item.expected.pResult); //-V521
            }
            else
            {
                REQUIRE(pResult == item.expected.pResult); //-V521
            }
        }
    }
    SECTION("parseMoneyFormat")
    {
        struct
        {
            const char* pStr;
            struct
            {
                string_view currency;
                char thousands;
                char append;

            } expected;
        }
        data[]
        {
            { ""       , { "$"  , ',', 0 } },
            { "''"     , { "$"  , ',', 0 } },
            { "'P'"    , { "P"  , ',', 0 } },
            { "'P',"   , { "P"  , ',', 0 } },
            { "'Q'."   , { "Q"  , '.', 0 } },
            { ".' E'"  , { " E" , '.', 1 } },
            { "'R$'.," , { "R$" , '.', 0 } }

        };

        for (auto& item : data)
        {
            Money::Optima::parseMoneyFormat(item.pStr);
            REQUIRE(item.expected.currency  == Money::Optima::currencySymbol); //-V521
            REQUIRE(item.expected.thousands == Money::Optima::thousandsSymbol); //-V521
            REQUIRE(item.expected.append    == Money::Optima::appendCurrency); //-V521
        }
    }
    SECTION("formatDollars")
    {
        struct
        {
            const char* pFormat;
            __int64     amount;
            string_view expected;
        }
        data[]
        {
            { ""       , 0, "$0"  },
            { "'P'"    , 0, "P0"  },
            { ".' E'"  , 0, "0 E" },
            { "'R$'.," , 0, "R$0" },

            { ""       , 12340000, "$1,234"  },
            { "'P'"    , 12340000, "P1,234"  },
            { ".' E'"  , 12340000, "1.234 E" },
            { "'R$'.," , 12340000, "R$1.234" },

            { ""       , -12340000, "-$1,234"  },
            { "'P'"    , -12340000, "-P1,234"  },
            { ".' E'"  , -12340000, "-1.234 E" },
            { "'R$'.," , -12340000, "-R$1.234" },
        };

        char buffer[50];

        for (auto& item : data)
        {
            Money::Optima::parseMoneyFormat(item.pFormat);
            REQUIRE(Money::Optima::formatDollars(item.amount, buffer, sizeof(buffer)) == item.expected); //-V521
        }
    }
}