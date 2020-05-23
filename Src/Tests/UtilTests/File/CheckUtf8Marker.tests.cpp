// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/File/CheckUtf8Marker.h"
#include "TestHelper/catch.hpp"
#include <sstream>

using namespace std;

TEST_CASE("File/CheckUtf8Marker")
{
    SECTION("good")
    {
        stringstream ss;
        stringbuf* pbuf = ss.rdbuf();
        pbuf->sputn("\xEF\xBB\xBF", 3);

        string s;
        bool isUtf8 = CheckUtf8Marker(ss, s);
        REQUIRE(s.empty()); //-V521
        REQUIRE(isUtf8); //-V521

        char ch;
        ss.get(ch);
        REQUIRE_FALSE(ss.get(ch)); //-V521
    }
    SECTION("data")
    {
        struct
        {
            string data;
            struct
            {
                string str;
                char nextChar;
                bool isUtf8;
                bool isEnd;

            } expected;
        }
        data[]
        {
            { "a"            , { ""             , 'a', false, false } },
            { "gh"           , { ""             , 'g', false, false } },
            { "\xEF"         , { "\xEF"         ,  0,  false, true  } },
            { "\xEFz"        , { "\xEF"         , 'z', false, false } },
            { "\xEF\xBBz"    , { "\xEF\xBB"     , 'z', false, false } },
            { "\xEF\xBB\xBFz", { ""             , 'z', true , false } },
            { "\xEF\xBB\xBF" , { ""             ,  0 , true , true  } },
            { ""             , { ""             ,  0 , false, true  } },
        };

        for (const auto& item : data)
        {
            stringstream ss;
            stringbuf* pbuf = ss.rdbuf();
            pbuf->sputn(item.data.data(), item.data.size());

            string s;
            bool isUtf8 = CheckUtf8Marker(ss, s);
            REQUIRE(s == item.expected.str); //-V521
            REQUIRE(isUtf8 == item.expected.isUtf8); //-V521

            char ch;
            ss.get(ch);

            if (item.expected.nextChar)
            {
                REQUIRE(ch == item.expected.nextChar); //-V521
            }
            else if (item.expected.isEnd)
            {
                REQUIRE_FALSE(ss.get(ch)); //-V521
            }
        }
    }
}

/*
#include <string>
#include <istream>

using namespace std;

string GetOrDiscardBOM(istream& stream)
{
    string str;

    // UTF-8 byte order mark
    // 0xEF, 0xBB, 0xBF
    int bom[]{ 0xEF, 0xBB, 0xBF };

    for (int ch : bom)
    {
        if (stream.eof() || stream.peek() != ch)
        {
            break;
        }

        str.push_back(static_cast<char>(stream.get()));
    }

    if (str.size() == _countof(bom))
    {
        str.resize(0);
    }

    return str;
}
*/