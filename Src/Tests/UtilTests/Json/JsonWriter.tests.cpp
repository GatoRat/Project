// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <sstream>
#include "Util/Json/JsonWriter.h"
#include "Util/String/Str.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("Json/JsonWriter")
{
    JsonWriter writer;

    SECTION("optionalKeyVal")
    {
        SECTION("optional<bool>")
        {
            optional<bool> valExists = true;
            writer.optionalKeyVal("exists", valExists);

            optional<bool> valEmpty;
            writer.optionalKeyVal("empty" , valEmpty);

            auto jsonStr = writer.getString();

            REQUIRE(jsonStr == R"({"exists":true})"); //-V521
        }
    }
}
