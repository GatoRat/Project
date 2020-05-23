// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/File/CsvCell.h"
#include "Util/File/XceptionFile.h"
#include "TestHelper/catch.hpp"
#include <sstream>

using namespace std;

TEST_CASE("File/CsvCell")
{
/*
    SECTION("find")
    {
        CsvFile csvFile("x,12,ba\nst,23,XY\nxy,45,gh");

        REQUIRE(csvFile.numRows() == 3);
        REQUIRE(csvFile.numColumns() == 3);

        auto cell = csvFile.find("xy", false);
        REQUIRE(cell.has_value());
        REQUIRE(cell->row == 2);
        REQUIRE(cell->column == 0);
        REQUIRE(cell->pStr == string_view("xy"));

        cell = csvFile.find("xy", true);
        REQUIRE(cell.has_value());
        REQUIRE(cell->row == 1);
        REQUIRE(cell->column == 2);
        REQUIRE(cell->pStr == string_view("XY"));

        cell = csvFile.find("xY", false);
        REQUIRE_FALSE(cell.has_value());
    }
    SECTION("findByRow")
    {
        CsvFile csvFile("x,12,ba\nst,23,XY\nxy,,gh");

        CsvFile::Cell cell;

        SECTION("success (wrap)")
        {
            csvFile.moveCellTo(cell, 0, 0);

            REQUIRE(csvFile.findByRow(cell, true, "xy", false, 0));
            REQUIRE(cell.column == 0);
            REQUIRE(cell.row == 2);
            REQUIRE(cell.pStr == string_view("xy"));
        }
        SECTION("failure (no wrap, case sensitive)")
        {
            csvFile.moveCellTo(cell, 0, 1);

            REQUIRE_FALSE(csvFile.findByRow(cell, false, "xy", false, 0));
            REQUIRE(cell.column == 0);
            REQUIRE(cell.row == 1);
            REQUIRE(cell.pStr == string_view("st"));
        }
        SECTION("success (no wrap, case insensitive)")
        {
            csvFile.moveCellTo(cell, 0, 1);

            REQUIRE(csvFile.findByRow(cell, false, "xy", true, 0));
            REQUIRE(cell.column == 2);
            REQUIRE(cell.row == 1);
            REQUIRE(cell.pStr == string_view("XY"));
        }
        SECTION("success (same cell)")
        {
            csvFile.moveCellTo(cell, 0, 1);

            REQUIRE(csvFile.findByRow(cell, false, "st", false, 0));
            REQUIRE(cell.column == 0);
            REQUIRE(cell.row == 1);
            REQUIRE(cell.pStr == string_view("st"));
        }
        SECTION("failure (next cell)")
        {
            csvFile.moveCellTo(cell, 1, 1);

            REQUIRE_FALSE(csvFile.findByRow(cell, false, "st", false, 0));
            REQUIRE(cell.column == 1);
            REQUIRE(cell.row == 1);
            REQUIRE(cell.pStr == string_view("23"));
        }
        SECTION("failure (empty string)")
        {
            csvFile.moveCellTo(cell, 0, 0);

            REQUIRE_FALSE(csvFile.findByRow(cell, false, "", false, 0));
            REQUIRE(cell.column == 0);
            REQUIRE(cell.row == 0);
            REQUIRE(cell.pStr == string_view("x"));
        }
        SECTION("success (empty string)")
        {
            csvFile.moveCellTo(cell, 0, 0);

            REQUIRE(csvFile.findByRow(cell, true, "", false, 0));
            REQUIRE(cell.column == 1);
            REQUIRE(cell.row == 2);
            REQUIRE(cell.pStr == string_view(""));
        }
    }
*/
    SECTION("ctor")
    {
        CsvFile csvFile("00,10,20\n"
                        "01,11,21\n"
                        "02,12,22");

        SECTION("0,0")
        {
            CsvCell cell(csvFile);
            REQUIRE(cell.column() == 0); //-V521
            REQUIRE(cell.row() == 0); //-V521
            REQUIRE(cell.str() == string_view("00")); //-V521
        }
        SECTION("2,2")
        {
            CsvCell cell(csvFile, 2, 2);
            REQUIRE(cell.column() == 2); //-V521
            REQUIRE(cell.row() == 2); //-V521
            REQUIRE(cell.str() == string_view("22")); //-V521
        }
        SECTION("0,3")
        {
            CsvCell cell(csvFile, 0, 3);
            REQUIRE(cell.column() == 0); //-V521
            REQUIRE(cell.row() == 2); //-V521
            REQUIRE(cell.str() == string_view("02")); //-V521
        }
        SECTION("3,0")
        {
            CsvCell cell(csvFile, 3, 0);
            REQUIRE(cell.column() == 2); //-V521
            REQUIRE(cell.row() == 0); //-V521
            REQUIRE(cell.str() == string_view("20")); //-V521
        }
        SECTION("3,3")
        {
            CsvCell cell(csvFile, 3, 3);
            REQUIRE(cell.column() == 2); //-V521
            REQUIRE(cell.row() == 2); //-V521
            REQUIRE(cell.str() == string_view("22")); //-V521
        }
    }
    SECTION("equals")
    {
        CsvFile csvFile(
            "00,10,20\n"
            "01,11,21\n"
            "02,12,22");

        CsvCell cell1(csvFile, 1, 2); //-V525
        CsvCell cell2(csvFile, 2, 2);
        CsvCell cell3(csvFile, 3, 3);

        REQUIRE(cell2 == cell3); //-V521
        REQUIRE_FALSE(cell1 == cell2); //-V521
    }
    SECTION("moveTo")
    {
        CsvFile csvFile(
            "00,10,20\n"
            "01,11,21\n"
            "02,12,22");

        CsvCell cell(csvFile);
        REQUIRE(cell.column() == 0); //-V521
        REQUIRE(cell.row() == 0); //-V521
        REQUIRE(cell.str() == string_view("00")); //-V521

        SECTION("2,2")
        {
            REQUIRE(cell.moveTo(2, 2) == string_view("22")); //-V521
            REQUIRE(cell.column() == 2); //-V521
            REQUIRE(cell.row() == 2); //-V521
            REQUIRE(cell.str() == string_view("22")); //-V521
        }
        SECTION("0,3")
        {
            REQUIRE(cell.moveTo(0, 3) == nullptr); //-V521
            REQUIRE(cell.column() == 0); //-V521
            REQUIRE(cell.row() == 2); //-V521
            REQUIRE(cell.str() == string_view("02")); //-V521
        }
        SECTION("3,0")
        {
            REQUIRE(cell.moveTo(3, 0) == nullptr); //-V521
            REQUIRE(cell.column() == 2); //-V521
            REQUIRE(cell.row() == 0); //-V521
            REQUIRE(cell.str() == string_view("20")); //-V521
        }
        SECTION("3,3")
        {
            REQUIRE(cell.moveTo(3, 3) == nullptr); //-V521
            REQUIRE(cell.column() == 2); //-V521
            REQUIRE(cell.row() == 2); //-V521
            REQUIRE(cell.str() == string_view("22")); //-V521
        }
    }
    SECTION("strAt")
    {
        CsvFile csvFile(
            "00,10,20\n"
            "01,11,21\n"
            "02,12,22");

        CsvCell cell(csvFile);

        SECTION("2,2")
        {
            REQUIRE(cell.strAt(2, 2) == string_view("22")); //-V521
        }
        SECTION("0,3")
        {
            REQUIRE(cell.strAt(0, 3) == nullptr); //-V521
        }
        SECTION("3,0")
        {
            REQUIRE(cell.strAt(3, 0) == nullptr); //-V521
        }
        SECTION("3,3")
        {
            REQUIRE(cell.strAt(3, 3) == nullptr); //-V521
        }

        REQUIRE(cell.column() == 0); //-V521
        REQUIRE(cell.row() == 0); //-V521
        REQUIRE(cell.str() == string_view("00")); //-V521
    }
    SECTION("strRelative")
    {
        CsvFile csvFile(
            "00,10,20\n"
            "01,11,21\n"
            "02,12,22");

        CsvCell cell(csvFile);
        cell.moveTo(1, 1);

        struct
        {
            size_t startColumn;
            size_t startRow;
            intmax_t relativeColumn;
            intmax_t relativeRow;
            const char* pExpected;
        }
        data[] =
        {
            { 1, 1,-2,-2, nullptr },
            { 1, 1,-2,-1, nullptr },
            { 1, 1,-2, 0, nullptr },
            { 1, 1,-2, 1, nullptr },
            { 1, 1,-2, 2, nullptr },

            { 1, 1,-1,-2, nullptr },
            { 1, 1,-1,-1, "00"    },
            { 1, 1,-1, 0, "01"    },
            { 1, 1,-1, 1, "02"    },
            { 1, 1,-1, 2, nullptr },

            { 1, 1, 0,-2, nullptr },
            { 1, 1, 0,-1, "10"    },
            { 1, 1, 0, 0, "11"    },
            { 1, 1, 0, 1, "12"    },
            { 1, 1, 0, 2, nullptr },

            { 1, 1, 1,-2, nullptr },
            { 1, 1, 1,-1, "20"    },
            { 1, 1, 1, 0, "21"    },
            { 1, 1, 1, 1, "22"    },
            { 1, 1, 1, 2, nullptr },

            { 1, 1, 2,-2, nullptr },
            { 1, 1, 2,-1, nullptr },
            { 1, 1, 2, 0, nullptr },
            { 1, 1, 2, 1, nullptr },
            { 1, 1, 2, 2, nullptr },
        };

        for (const auto& item : data)
        {
            cell.moveTo(item.startColumn, item.startRow);

            if (item.pExpected)
            {
                REQUIRE(cell.strRelative(item.relativeColumn, item.relativeRow) == string_view(item.pExpected)); //-V521
            }
            else
            {
                REQUIRE(cell.strRelative(item.relativeColumn, item.relativeRow) == nullptr); //-V521
            }

            REQUIRE(cell.column() == item.startColumn); //-V521
            REQUIRE(cell.row() == item.startRow); //-V521
        }
    }
    SECTION("nav")
    {
        CsvFile csvFile(
            "00,10,20,30,40,50\n"
            "01,11,21,31,41,51\n"
            "02,12,22,32,42\n"
            "03,13,23,33,43,53\n"
            "04,14,24,34,44,54\n"
            "05,15,25,35,45,55\n"
        );

        SECTION("move")
        {
            CsvCell cell(csvFile, 9, 9);

            REQUIRE(cell.left(3) == string_view("25")); //-V521
            REQUIRE(cell.left(1) == string_view("15")); //-V521
            REQUIRE(cell.left(1) == string_view("05")); //-V521
            REQUIRE(cell.left(1) == nullptr); //-V521
            REQUIRE(cell.str() == string_view("05")); //-V521
            REQUIRE(cell.left(10) == nullptr); //-V521
            REQUIRE(cell.str() == string_view("05")); //-V521

            REQUIRE(cell.up(3) == string_view("02")); //-V521
            REQUIRE(cell.up(1) == string_view("01")); //-V521
            REQUIRE(cell.up(1) == string_view("00")); //-V521
            REQUIRE(cell.up(1) == nullptr); //-V521
            REQUIRE(cell.str() == string_view("00")); //-V521
            REQUIRE(cell.up(10) == nullptr); //-V521
            REQUIRE(cell.str() == string_view("00")); //-V521

            REQUIRE(cell.right(3) == string_view("30")); //-V521
            REQUIRE(cell.right(1) == string_view("40")); //-V521
            REQUIRE(cell.right(1) == string_view("50")); //-V521
            REQUIRE(cell.right(1) == nullptr); //-V521
            REQUIRE(cell.str() == string_view("50")); //-V521
            REQUIRE(cell.right(10) == nullptr); //-V521
            REQUIRE(cell.str() == string_view("50")); //-V521

            REQUIRE(cell.down(3) == string_view("53")); //-V521
            REQUIRE(cell.down(1) == string_view("54")); //-V521
            REQUIRE(cell.down(1) == string_view("55")); //-V521
            REQUIRE(cell.down(1) == nullptr); //-V521
            REQUIRE(cell.str() == string_view("55")); //-V521
            REQUIRE(cell.down(10) == nullptr); //-V521
            REQUIRE(cell.str() == string_view("55")); //-V521

            REQUIRE(cell.moveBy(-3, -100) == nullptr); //-V521
            REQUIRE(cell.str() == string_view("20")); //-V521
            REQUIRE(cell.moveBy(5, 0) == nullptr); //-V521
            REQUIRE(cell.str() == string_view("50")); //-V521

            REQUIRE(cell.down(1) == string_view("51")); //-V521
            REQUIRE(cell.down(1) == nullptr); //-V521
            REQUIRE(cell.str() == string_view("42")); //-V521
            REQUIRE(cell.down(1) == string_view("43")); //-V521

            REQUIRE(cell.moveTo(5, 2) == nullptr); //-V521
            REQUIRE(cell.str() == string_view("42")); //-V521
        }
        SECTION("str")
        {
            CsvCell cell(csvFile, 2, 2);

            REQUIRE(cell.str() == string_view("22")); //-V521

            REQUIRE(cell.strLeft(0) == string_view("22")); //-V521
            REQUIRE(cell.strLeft(1) == string_view("12")); //-V521
            REQUIRE(cell.strLeft(2) == string_view("02")); //-V521
            REQUIRE(cell.strLeft(3) == nullptr); //-V521

            REQUIRE(cell.strRight(0) == string_view("22")); //-V521
            REQUIRE(cell.strRight(1) == string_view("32")); //-V521
            REQUIRE(cell.strRight(2) == string_view("42")); //-V521
            REQUIRE(cell.strRight(3) == nullptr); //-V521
            REQUIRE(cell.strRight(4) == nullptr); //-V112 //-V521

            REQUIRE(cell.strUp(0) == string_view("22")); //-V521
            REQUIRE(cell.strUp(1) == string_view("21")); //-V521
            REQUIRE(cell.strUp(2) == string_view("20")); //-V521
            REQUIRE(cell.strUp(3) == nullptr); //-V521

            REQUIRE(cell.strDown(0) == string_view("22")); //-V521
            REQUIRE(cell.strDown(1) == string_view("23")); //-V521
            REQUIRE(cell.strDown(2) == string_view("24")); //-V521
            REQUIRE(cell.strDown(3) == string_view("25")); //-V521
            REQUIRE(cell.strDown(4) == nullptr); //-V112 //-V521
        }
    }
    SECTION("scanByRow")
    {
        CsvFile csvFile(
            "00,10,20,30,40\n"
            "01,11,21,31\n"
            "02,12,22,32,42"
        );

        bool visited[3][5]
        {
            { false, false, false, false, false },
            { false, false, false, false, false },
            { false, false, false, false, false }
        };

        CsvCell cell(csvFile);

        SECTION("Wrap")
        {
            const char* pScanByRowResult = cell.scanByRow([&](const char* pStr, size_t column, size_t row)->CsvCell::ScanCallbackReturn
            {
                REQUIRE_FALSE(visited[row][column]); //-V521
                visited[row][column] = true;

                stringstream expected;
                expected << column << row;
                REQUIRE(pStr == expected.str()); //-V521

                return CsvCell::Continue;
            }, CsvCell::Wrap, false);

            REQUIRE(pScanByRowResult == nullptr); //-V521
            REQUIRE(cell.column() == 0); //-V521
            REQUIRE(cell.row() == 0); //-V521
            REQUIRE(cell.str() == string_view("00")); //-V521

            REQUIRE_FALSE(visited[0][0]); //-V521
            visited[0][0] = true;

            REQUIRE_FALSE(visited[1][4]); //-V521
            visited[1][4] = true;

            for (size_t row = 0; row < 3; ++row)
            {
                for (size_t column = 0; column < 5; ++column)
                {
                    REQUIRE(visited[row][column]); //-V521
                }
            }
        }
        SECTION("Wrap (include current cell)")
        {
            const char* pScanByRowResult = cell.scanByRow([&](const char* pStr, size_t column, size_t row)->CsvCell::ScanCallbackReturn
            {
                REQUIRE_FALSE(visited[row][column]); //-V521
                visited[row][column] = true;

                stringstream expected;
                expected << column << row;
                REQUIRE(pStr == expected.str()); //-V521

                return CsvCell::Continue;
            }, CsvCell::Wrap, true);

            REQUIRE(pScanByRowResult == nullptr); //-V521
            REQUIRE(cell.column() == 0); //-V521
            REQUIRE(cell.row() == 0); //-V521
            REQUIRE(cell.str() == string_view("00")); //-V521

            REQUIRE_FALSE(visited[1][4]); //-V521
            visited[1][4] = true;

            for (size_t row = 0; row < 3; ++row)
            {
                for (size_t column = 0; column < 5; ++column)
                {
                    REQUIRE(visited[row][column]); //-V521
                }
            }
        }
        SECTION("NoWrap")
        {
            size_t expectedRow = cell.row();

            const char* pScanByRowResult = cell.scanByRow([&](const char* pStr, size_t column, size_t row)->CsvCell::ScanCallbackReturn
            {
                REQUIRE(row == expectedRow); //-V521
                REQUIRE_FALSE(visited[row][column]); //-V521
                visited[row][column] = true;

                stringstream expected;
                expected << column << row;
                REQUIRE(pStr == expected.str()); //-V521

                return CsvCell::Continue;

            }, CsvCell::NoWrap, false);

            REQUIRE(pScanByRowResult == nullptr); //-V521
            REQUIRE(cell.column() == 0); //-V521
            REQUIRE(cell.row() == 0); //-V521
            REQUIRE(cell.str() == string_view("00")); //-V521

            REQUIRE_FALSE(visited[0][0]); //-V521
            visited[0][0] = true;

            for (size_t column = 0; column < 5; ++column)
            {
                REQUIRE(visited[expectedRow][column]); //-V521
            }
        }
        SECTION("NoWrap")
        {
            size_t expectedRow = cell.row();

            const char* pScanByRowResult = cell.scanByRow([&](const char* pStr, size_t column, size_t row)->CsvCell::ScanCallbackReturn
            {
                REQUIRE(row == expectedRow); //-V521
                REQUIRE_FALSE(visited[row][column]); //-V521
                visited[row][column] = true;

                stringstream expected;
                expected << column << row;
                REQUIRE(pStr == expected.str()); //-V521

                return CsvCell::Continue;

            }, CsvCell::NoWrap, true);

            REQUIRE(pScanByRowResult == nullptr); //-V521
            REQUIRE(cell.column() == 0); //-V521
            REQUIRE(cell.row() == 0); //-V521
            REQUIRE(cell.str() == string_view("00")); //-V521

            for (size_t column = 0; column < 5; ++column)
            {
                REQUIRE(visited[expectedRow][column]); //-V521
            }
        }
    }
    SECTION("findNext")
    {
        CsvFile csvFile(
            "xY,,xy,xy,xy\n"
            "XY,XY,XY,\n"
            ",,Xy,Xy,xY"
        );

        CsvCell cell(csvFile, 0, 0);

        SECTION("ByRow")
        {
            REQUIRE(cell.findNext("xY", CsvCell::ByRow) == string_view("xY")); //-V521
            REQUIRE(cell.column() == 4); //-V112 //-V521
            REQUIRE(cell.row() == 2); //-V521

            cell.moveTo(1, 0);

            REQUIRE(cell.findNext("", CsvCell::ByRow) == string_view("")); //-V521
            REQUIRE(cell.column() == 3); //-V521
            REQUIRE(cell.row() == 1); //-V521

            REQUIRE(cell.findNext(nullptr, CsvCell::ByRow) == string_view("Xy")); //-V521
            REQUIRE(cell.column() == 2); //-V521
            REQUIRE(cell.row() == 2); //-V521
        }
        SECTION("ByColumn")
        {
            REQUIRE(cell.findNext("xY", CsvCell::ByColumn) == string_view("xY")); //-V521
            REQUIRE(cell.column() == 4); //-V112 //-V521
            REQUIRE(cell.row() == 2); //-V521

            cell.moveTo(0, 0);

            REQUIRE(cell.findNext("", CsvCell::ByColumn) == string_view("")); //-V521
            REQUIRE(cell.column() == 0); //-V521
            REQUIRE(cell.row() == 2); //-V521

            REQUIRE(cell.findNext(nullptr, CsvCell::ByColumn) == string_view("XY")); //-V521
            REQUIRE(cell.column() == 1); //-V521
            REQUIRE(cell.row() == 1); //-V521
        }
    }
}
