// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <sstream>
#include <RapidJSON/pointer.h>
#include "Util/Json/JsonGet.h"
#include "Util/String/Str.h"
#include "TestHelper/catch.hpp"

using namespace std;

static const char* pJson =
R"({
  "s" : "str",
  "s1": "",
  "t" : true,
  "f" : false,
  "n" : null,
  "i" : 12,
  "pi": 3.14,
  "a" : [1, 2],
  "a1": [],
  "o2": { "i" : 12 }
})";

TEST_CASE("Json/JsonGet")
{
    rapidjson::Document jsonDoc;
    if (jsonDoc.Parse(pJson).HasParseError())
    {
        Str::LineColumn lineColumn(pJson, jsonDoc.GetErrorOffset());
        stringstream ss;
        ss << "Parse error: " << jsonDoc.GetParseError() << " Line=" << lineColumn.line << " Column=" << lineColumn.column;
        FAIL(ss.str());
    }

    SECTION("JsonGetString")
    {
        SECTION("empty")
        {
            const char* pStr = JsonGetString(jsonDoc, "s1", "bogus");
            REQUIRE(*pStr == 0); //-V521
        }
        SECTION("full")
        {
            const char* pStr = JsonGetString(jsonDoc, "s", "");
            REQUIRE(strcmp(pStr, "str") == 0); //-V521
        }
        SECTION("null")
        {
            const char* pStr = JsonGetString(jsonDoc, "n", "null");
            REQUIRE(strcmp(pStr, "null") == 0); //-V521
        }
        SECTION("null2")
        {
            auto memberIt = jsonDoc.FindMember("n");
            REQUIRE(memberIt != jsonDoc.MemberEnd()); //-V521
            REQUIRE(memberIt->value.IsNull()); //-V521
        }
        SECTION("null object")
        {
            const rapidjson::Value* pValue = rapidjson::Pointer("/n").Get(jsonDoc);
            REQUIRE(pValue->IsNull()); //-V521
            REQUIRE(!pValue->IsObject()); //-V521
        }
    }
}
