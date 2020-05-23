// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <sstream>

#include "Util/File/CsvFile.h"
#include "Util/File/XceptionFile.h"
#include "TestHelper/catch.hpp"

using namespace std;

TEST_CASE("File/CsvData")
{
    SECTION("parse1")
    {
        CsvFile csvFile("\"a\"\"b \",,c\n1,2,3");

        REQUIRE(csvFile.numRows() == 2); //-V521
        REQUIRE(csvFile.numColumns() == 3); //-V521

        REQUIRE(csvFile[0][0] == string_view("a\"b ")); //-V521
        REQUIRE(csvFile[0][1] == string_view("")); //-V521
        REQUIRE(csvFile[0][2] == string_view("c")); //-V521
        REQUIRE(csvFile[1][0] == string_view("1")); //-V521
        REQUIRE(csvFile[1][1] == string_view("2")); //-V521
        REQUIRE(csvFile[1][2] == string_view("3")); //-V521
    }
    SECTION("parse2")
    {
        CsvFile csvFile("\"a\"\"b \",,c\n1,2");

        REQUIRE(csvFile.numRows() == 2); //-V521
        REQUIRE(csvFile.numColumns() == 3); //-V521

        REQUIRE(csvFile[0][0] == string_view("a\"b ")); //-V521
        REQUIRE(csvFile[0][1] == string_view("")); //-V521
        REQUIRE(csvFile[0][2] == string_view("c")); //-V521
        REQUIRE(csvFile[1][0] == string_view("1")); //-V521
        REQUIRE(csvFile[1][1] == string_view("2")); //-V521

        REQUIRE_FALSE(csvFile.checkRowLengths()); //-V521
        csvFile.normalizeRowLengths();
        REQUIRE(csvFile.checkRowLengths()); //-V521
        REQUIRE(csvFile[1][2] == string_view("")); //-V521
    }
    SECTION("parse3")
    {
        CsvFile csvFile("\"ab\nc\",,d\r\nx \r\n1,2,\"3\"");

        REQUIRE_FALSE(csvFile.checkRowLengths()); //-V521
        csvFile.normalizeRowLengths();
        REQUIRE(csvFile.checkRowLengths()); //-V521

        REQUIRE(csvFile.numRows() == 3); //-V521
        REQUIRE(csvFile.numColumns() == 3); //-V521

        REQUIRE(csvFile[0][0] == string_view("ab\nc")); //-V521
        REQUIRE(csvFile[0][1] == string_view("")); //-V521
        REQUIRE(csvFile[0][2] == string_view("d")); //-V521

        REQUIRE(csvFile[1][0] == string_view("x ")); //-V521
        REQUIRE(csvFile[1][1] == string_view("")); //-V521
        REQUIRE(csvFile[1][1] == string_view("")); //-V521

        REQUIRE(csvFile[2][0] == string_view("1")); //-V521
        REQUIRE(csvFile[2][1] == string_view("2")); //-V521
        REQUIRE(csvFile[2][2] == string_view("3")); //-V521

        stringstream ss;
        csvFile.write(ss);
        string output(ss.str());

        REQUIRE(output == "\"ab\nc\",,d\nx ,,\n1,2,3\n"); //-V521
    }
    SECTION("parse4")
    {
        CsvFile csvFile("a1, a2 ,a3");

        REQUIRE(csvFile.numRows() == 1); //-V521
        REQUIRE(csvFile.numColumns() == 3); //-V521

        csvFile.trimCells();

        REQUIRE(csvFile.numRows() == 1); //-V521
        REQUIRE(csvFile.numColumns() == 3); //-V521

        REQUIRE(csvFile[0][0] == string_view("a1")); //-V521
        REQUIRE(csvFile[0][1] == string_view("a2")); //-V521
        REQUIRE(csvFile[0][2] == string_view("a3")); //-V521
    }
}
