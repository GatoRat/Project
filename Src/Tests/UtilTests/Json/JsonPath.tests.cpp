// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Json/JsonPath.h"
#include <string_view>
#include <RapidJSON/pointer.h>
#include "Util/Json/JsonFormatError.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Json/JsonPath")
{
    SECTION("isEmpty")
    {
        JsonPath jsonPath("");
        REQUIRE(jsonPath.isEmpty()); //-V521
    }
    SECTION("ctor (1) & append & index operator")
    {
        struct
        {
            string_view param;
            vector<string_view> expected;
        }
        data[] =
        {
            { "val/"         , { "val"       } },
            { ""             ,                 },
            { "/"            ,                 },
            { "val"          , { "val"       } },
            { "/val"         , { "val"       } },
            { "/val/"        , { "val"       } },
            { "p1/val"       , { "p1", "val" } },
            { "/p1/val"      , { "p1", "val" } },
            { "/p1/val//"    , { "p1", "val" } },
            { "/p1//p2/val//", { "p1", "p2", "val" } },
        };

        for (const auto& entry : data)
        {
            JsonPath jsonPath(entry.param);
            REQUIRE(jsonPath.size() == entry.expected.size()); //-V521
            if (!jsonPath.isEmpty())
            {
                for (size_t i = 0; i < jsonPath.size(); ++i)
                {
                    REQUIRE(jsonPath[i] == entry.expected[i]); //-V521
                }
            }
        }
    }
    SECTION("ctor (2) & append & index operator")
    {
        struct
        {
            string_view param1;
            string_view param2;
            vector<string_view> expected;
        }
        data[] =
        {
            { ""         , ""          ,                 },
            { ""         , "/"         ,                 },
            { ""         , "val"       , { "val"       } },
            { ""         , "/val"      , { "val"       } },
            { ""         , "/val/"     , { "val"       } },

            { "/"        , ""          ,                 },
            { "/"        , "/"         ,                 },
            { "/"        , "val"       , { "val"       } },
            { "/"        , "/val"      , { "val"       } },
            { "/"        , "/val/"     , { "val"       } },

            { "p1"        , ""         , { "p1"        } },
            { "p1"        , "/"        , { "p1"        } },
            { "p1"        , "val"      , { "p1", "val" } },
            { "p1"        , "/val"     , { "p1", "val" } },
            { "p1"        , "/val/"    , { "p1", "val" } },

            { "/p1"       , ""         , { "p1"        } },
            { "/p1"       , "/"        , { "p1"        } },
            { "/p1"       , "val"      , { "p1", "val" } },
            { "/p1"       , "/val"     , { "p1", "val" } },
            { "/p1"       , "/val/"    , { "p1", "val" } },

            { "/p1/p2"    , "/val/"    , { "p1", "p2", "val" } },
            { "/p1"       , "p2/val/"  , { "p1", "p2", "val" } },
        };

        for (const auto& entry : data)
        {
            JsonPath jsonPath(entry.param1, entry.param2);
            REQUIRE(jsonPath.size() == entry.expected.size()); //-V521
            if (!jsonPath.isEmpty())
            {
                for (size_t i = 0; i < jsonPath.size(); ++i)
                {
                    REQUIRE(jsonPath[i] == entry.expected[i]); //-V521
                }
            }
        }
    }
    SECTION("operator+=")
    {
        JsonPath jsonPath("p1");
        REQUIRE((jsonPath += "val").toPointer() == string_view("/p1/val")); //-V521
    }
    SECTION("removeEnd")
    {
        JsonPath jsonPath("/p1/val");
        REQUIRE(jsonPath.removeEnd().toPointer() == string_view("/p1")); //-V807 //-V521
        REQUIRE(jsonPath.removeEnd().toPointer() == string_view("")); //-V521
    }
    SECTION("replaceEnd")
    {
        SECTION("two")
        {
            JsonPath jsonPath("/p1/v1");
            REQUIRE(jsonPath.replaceEnd("v2").toPointer() == string_view("/p1/v2")); //-V521
        }
        SECTION("two + empty")
        {
            JsonPath jsonPath("/p1/v1");
            REQUIRE(jsonPath.replaceEnd("").toPointer() == string_view("/p1")); //-V521
        }
        SECTION("one")
        {
            JsonPath jsonPath("/p1");
            REQUIRE(jsonPath.replaceEnd("v2").toPointer() == string_view("/v2")); //-V521
        }
        SECTION("empty")
        {
            JsonPath jsonPath("");
            REQUIRE(jsonPath.replaceEnd("v2").toPointer() == string_view("/v2")); //-V521
        }
    }
    SECTION("find")
    {
        JsonPath jsonPath("/p1/p2/val");
        REQUIRE(jsonPath.find("p2") == 1); //-V521
        REQUIRE(jsonPath.find("p3") == JsonPath::npos); //-V521
    }
    SECTION("GetLastToken")
    {
        struct
        {
            string_view param;
            string_view expected;
        }
        data[] =
        {
            { ""             , ""    },
            { "/"            , ""    },
            { "val"          , "val" },
            { "/val"         , "val" },
            { "/val/"        , "val" },
            { "/p1/p2/val//" , "val" },
        };

        for (const auto& entry : data)
        {
            REQUIRE(JsonPath::GetLastToken(entry.param) == entry.expected); //-V521
        }
    }
    SECTION("get")
    {
        const char* pJson =
R"J({
"base":
{
  "k":-1,
  "a":
  {
    "b":
    {
      "k":2
    },
    "c":
    {
        "d":
        {
        }
    },
    "k":1
  },
  "x":
  {
    "y":null
  },
  "z":null,
  "r": [ 99 ]
},
"3x2":
{
}
})J";
        rapidjson::Document doc;
        if (doc.Parse(pJson).HasParseError())
        {
            string errStr = JsonFormatError(doc, "pJson", pJson, __FUNCTION__, __LINE__);
            FAIL(errStr);
        }

        const rapidjson::Value* pBase = rapidjson::GetValueByPointer(doc, "/base");
        REQUIRE(pBase); //-V521
        {
            JsonPath jsonPath("k");
            auto val = jsonPath.get(pBase);
            REQUIRE(val.isValid()); //-V521
            REQUIRE(val.pValue->IsInt()); //-V521
            REQUIRE(val.pValue->GetInt() == -1); //-V521
        }
        {
            JsonPath jsonPath("z");
            auto val = jsonPath.get(pBase);
            REQUIRE(val.haltSearch); //-V521
        }
        {
            JsonPath jsonPath("z/a");
            auto val = jsonPath.get(pBase);
            REQUIRE(val.haltSearch); //-V521
        }
        {
            JsonPath jsonPath("r");
            auto val = jsonPath.get(pBase);
            REQUIRE(val.isValid()); //-V521
            REQUIRE(val.pValue->IsArray()); //-V521
        }
        {
            JsonPath jsonPath("x/y");
            auto val = jsonPath.get(pBase);
            REQUIRE(val.haltSearch); //-V521
        }
        {
            JsonPath jsonPath("r/s");
            auto val = jsonPath.get(pBase);
            REQUIRE(val.haltSearch); //-V521
        }
        {
            JsonPath jsonPath("a/b");
            auto val = jsonPath.search(pBase, "k");
            REQUIRE(val.isValid()); //-V521
            REQUIRE(val.pValue->IsInt()); //-V521
            REQUIRE(val.pValue->GetInt() == 2); //-V521
        }
        {
            JsonPath jsonPath("a");
            auto val = jsonPath.search(pBase, "b/k");
            REQUIRE(val.isValid()); //-V521
            REQUIRE(val.pValue->IsInt()); //-V521
            REQUIRE(val.pValue->GetInt() == 2); //-V521
        }
        {
            JsonPath jsonPath("a/b/c/d/e");
            auto val = jsonPath.search(pBase, "k");
            REQUIRE(val.isValid()); //-V521
            REQUIRE(val.pValue->IsInt()); //-V521
            REQUIRE(val.pValue->GetInt() == 2); //-V521
        }
        {
            JsonPath jsonPath("a//c/d/e");
            auto val = jsonPath.search(pBase, "k");
            REQUIRE(val.isValid()); //-V521
            REQUIRE(val.pValue->IsInt()); //-V521
            REQUIRE(val.pValue->GetInt() == 1); //-V521
        }
        {
            JsonPath jsonPath("x");
            auto val = jsonPath.search(pBase, "k");
            REQUIRE(val.isValid()); //-V521
            REQUIRE(val.pValue->IsInt()); //-V521
            REQUIRE(val.pValue->GetInt() == -1); //-V521
        }
        {
            JsonPath jsonPath("x");
            auto val = jsonPath.search(pBase, "k", false);
            REQUIRE(val.keepSearching()); //-V521
        }
    }
    SECTION("toFilename")
    {
        struct
        {
            string_view param;
            JsonPath::Filename expected;
        }
        data[] =
        {
            { ""               , { ""         , false } },
            { "/"              , { ""         , false } },
            { "val"            , { "val"      , false } },
            { "/val"           , { "val"      , false } },
            { "/val/"          , { "val"      , false } },
            { "/p1/p2/val//"   , { "p1.p2.val", false } },
            { "/p1/val/file/"  , { "p1.val"   , false } },
            { "/p1/val/files/" , { "p1.val"   , true  } },
            { "/p1/val/font/"  , { "p1.val"   , false } },
            { "/p1/val/fonts/" , { "p1.val"   , true  } },
            { "/p1/val/sheet/" , { "p1.val"   , false } },
            { "/p1/val/sheets/", { "p1.val"   , true  } },
        };

        for (const auto& entry : data)
        {
            auto filename = JsonPath(entry.param).toFilename();
            REQUIRE(filename.filename == entry.expected.filename); //-V521
            REQUIRE(filename.isPlural == entry.expected.isPlural); //-V521
        }
    }
}
