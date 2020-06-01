// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CsvCell.h"
#include <memory>
#include "../String/Str.h"

using namespace std;

CsvCell::CsvCell(const CsvFile& csvFile, size_t column, size_t row)
    : _csv(csvFile)
{
    moveTo(column, row);
}

bool CsvCell::operator==(const CsvCell& rhs) const
{
    return this == &rhs ||
        (_column == rhs._column && _row == rhs._row &&
         _pStr == rhs._pStr && addressof(_csv) == addressof(rhs._csv));
}

size_t CsvCell::column() const
{
    return _column;
}

size_t CsvCell::row() const
{
    return _row;
}

const char* CsvCell::str() const
{
    return _pStr;
}

bool CsvCell::isEmpty() const
{
    return Str::IsNullOrWhiteSpace(_pStr);
}

const char* CsvCell::moveBy(intmax_t columns, intmax_t rows)
{
    return moveToSigned(static_cast<intmax_t>(_column) + columns, static_cast<intmax_t>(_row) + rows);
}

const char* CsvCell::moveToSigned(intmax_t column, intmax_t row)
{
    const bool success = calcMove(column, row);

    _row = static_cast<size_t>(row);
    _column = static_cast<size_t>(column);
    _pStr = _csv.rows[_row][_column];

    return success ? _pStr : nullptr;
}

bool CsvCell::calcMove(intmax_t& column, intmax_t& row) const
{
    bool success = true;

    if (row >= static_cast<intmax_t>(_csv.rows.size()))
    {
        row = _csv.rows.size() - 1;
        success = false;
    }
    else if (row < 0)
    {
        row = 0;
        success = false;
    }

    if (column >= static_cast<intmax_t>(_csv.rows[row].size()))
    {
        column = _csv.rows[row].size() - 1;
        success = false;
    }
    else if (column < 0) //-V2516
    {
        column = 0;
        success = false;
    }

    return success;
}

const char* CsvCell::moveTo(size_t column, size_t row)
{
    return moveToSigned(static_cast<intmax_t>(column), static_cast<intmax_t>(row));
}

const char* CsvCell::strAt(size_t column, size_t row)
{
    CsvCell cell(*this);
    return cell.moveTo(column, row);
}

const char* CsvCell::strRelative(intmax_t numColumns, intmax_t numRows)
{
    CsvCell cell(*this);
    return cell.moveBy(numColumns, numRows);
}

const char* CsvCell::left(size_t numColumns)
{
    return moveBy(-static_cast<intmax_t>(numColumns), 0);
}

const char* CsvCell::right(size_t numColumns)
{
    return moveBy(static_cast<intmax_t>(numColumns), 0);
}

const char* CsvCell::up(size_t numRows)
{
    return moveBy(0, -static_cast<intmax_t>(numRows));
}

const char* CsvCell::down(size_t numRows)
{
    return moveBy(0, static_cast<intmax_t>(numRows));
}

const char* CsvCell::strLeft(size_t numColumns)
{
    return calcLeft(numColumns) ? _csv.rows[_row][numColumns] : nullptr;
}

const char* CsvCell::strRight(size_t numColumns)
{
    return calcRight(numColumns) ? _csv.rows[_row][numColumns] : nullptr;
}

const char* CsvCell::strUp(size_t numRows)
{
    return calcUp(numRows) ? _csv.rows[numRows][_column] : nullptr;
}

const char* CsvCell::strDown(size_t numRows)
{
    return calcDown(numRows) ? _csv.rows[numRows][_column] : nullptr;
}

bool CsvCell::calcLeft(size_t& numColumns) const
{
    bool success = true;

    if (numColumns > _column)
    {
        numColumns = 0;
        success = false;
    }
    else
    {
        numColumns = _column - numColumns;
    }

    return success;
}

bool CsvCell::calcRight(size_t& numColumns) const
{
    bool success = true;

    if (_column + numColumns >= _csv.rows[_row].size())
    {
        numColumns = _csv.rows[_row].size() - 1;
        success = false;
    }
    else
    {
        numColumns += _column;
    }

    return success;
}

bool CsvCell::calcUp(size_t& numRows) const
{
    bool success = true;

    if (numRows > _row)
    {
        numRows = 0;
        success = false;
    }
    else
    {
        numRows = _row - numRows;
    }

    return success;
}

bool CsvCell::calcDown(size_t& numRows) const
{
    bool success = true;

    if (_row + numRows >= _csv.rows.size())
    {
        numRows = _csv.rows.size() - 1;
        success = false;
    }
    else
    {
        numRows += _row;
    }

    return success;
}

const char* CsvCell::scanByRow(ScanCallback scanCallback, DoWrap wrap, bool includeCurCell) //-V2506
{
    if (scanCallback)
    {
        size_t column = includeCurCell ? _column : _column + 1;
        size_t row = _row;
        do
        {
            for (; column < _csv.rows[row].size(); ++column)
            {
                const char* pStr = _csv.rows[row][column];
                const ScanCallbackReturn result = scanCallback(pStr, column, row);
                if (result <= CancelWithFailure)
                {
                    return nullptr;
                }

                if (result >= CancelWithSuccess)
                {
                    _column = column;
                    _row = row;
                    _pStr = pStr;
                    return _pStr;
                }
            }

            if (wrap == NoWrap)
            {
                break;
            }

            column = 0;

        } while (++row < _csv.rows.size());
    }

    return nullptr;
}

const char* CsvCell::scanByColumn(ScanCallback scanCallback, DoWrap wrap, bool includeCurCell) //-V2506
{
    if (scanCallback)
    {
        const size_t maxRowLen = _csv.getMaxRowLength();
        size_t column = _column;
        size_t row = includeCurCell ? _row : _row + 1;
        do
        {
            for (; row < _csv.rows.size(); ++row)
            {
                if (column < _csv.rows[row].size())
                {
                    const char* pStr = _csv.rows[row][column];
                    const ScanCallbackReturn result = scanCallback(pStr, column, row);
                    if (result <= CancelWithFailure)
                    {
                        return nullptr;
                    }

                    if (result >= CancelWithSuccess)
                    {
                        _column = column;
                        _row = row;
                        _pStr = pStr;
                        return _pStr;
                    }
                }
            }

            if (wrap == NoWrap)
            {
                break;
            }

            row = 0;

        } while (++column < maxRowLen);
    }

    return nullptr;
}

const char* CsvCell::findNext(const char* pSearchStr, FindBy findBy, DoWrap wrap, bool ignoreCase, size_t len)
{
    return findBy == ByRow ?
        (pSearchStr == FirstNonEmpty ?
            scanByRow([&](const char* pStr, size_t /*column*/, size_t /*row*/)->ScanCallbackReturn
            {
                return *pStr ? CancelWithSuccess : Continue;
            }, wrap, false) :
            scanByRow([&](const char* pStr, size_t /*column*/, size_t /*row*/)->ScanCallbackReturn
            {
                return Str::Compare(pSearchStr, pStr, ignoreCase, len) == 0 ? CancelWithSuccess : Continue;
            }, wrap, false)
        ) :
        (pSearchStr == FirstNonEmpty ?
            scanByColumn([&](const char* pStr, size_t /*column*/, size_t /*row*/)->ScanCallbackReturn
            {
                return *pStr ? CancelWithSuccess : Continue;
            }, wrap, false) :
            scanByColumn([&](const char* pStr, size_t /*column*/, size_t /*row*/)->ScanCallbackReturn
            {
                return Str::Compare(pSearchStr, pStr, ignoreCase, len) == 0 ? CancelWithSuccess : Continue;
            }, wrap, false)
        );
}

string CsvCell::toString() const
{
    return toString(*this);
}

string CsvCell::toString(size_t column)
{
    string columnStr;

    ++column;

    do
    {
        --column;
        columnStr += static_cast<char>(column % 26 + 'A');
        column /= 26;

    } while (column);

    reverse(columnStr.begin(), columnStr.end());
    return columnStr;
}

string CsvCell::toString(size_t column, size_t row)
{
    return toString(column) + to_string(row + 1);
}

string CsvCell::toString(const CsvCell& cell)
{
    return toString(cell.column(), cell.row());
}
