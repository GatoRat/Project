// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <functional>
#include "CsvFile.h"

/// <summary>
/// Class to move around a CvsFile table.
/// </summary>
/// <remarks>
/// The CvsFile instance must remain valid while the CvsCell instance associated
/// with it is in use. The CvsFile should be normalized before being passed to
/// one of these instances. Failure to do so may result in unpredictable behavior
/// when this class is used at the end of rows.
///
/// This class does not handle tables with empty rows; using such a table may
/// result in out_of_range exceptions. Again, this can be avoided by normalizing
/// the table first.
///
/// A cell instance will always be valid (within the constraints of the table.)
/// An error is indicated by a nullptr return; there is no other indicator of an
/// error.
///
/// If this class is derived, the copy should be kept as fast and lightweight as
/// possible. If the default copy constructor or assignment operator cannot be used
/// the class is very likely too big and/or complicated.
/// </remarks>
class CsvCell
{
public:
    explicit CsvCell(const CsvFile& csvFile, size_t column = 0, size_t row = 0);
    virtual ~CsvCell() = default;

    bool operator==(const CsvCell& rhs) const;

    size_t column() const;
    size_t row() const;
    const char* str() const;

    bool isEmpty() const;

    const char* moveBy(intmax_t column, intmax_t row);

    /// <remarks>
    /// This will resolve row first, and then column. If row exceeds row count,
    /// it will be set to the last row. If column exceeds column count for that
    /// row, the column will be set to the last column of that row.
    ///
    /// This has implications for tables which haven't been normalized. For example,
    /// if a table has two rows with 10 columns on the first row and 3 on the second,
    /// moveto(3, 9) will result in the cell being on cell 1, 2
    /// </remarks>
    const char* moveTo(size_t column, size_t row);

    const char* strAt(size_t column, size_t row);
    const char* strRelative(intmax_t numColumns, intmax_t numRows);

    const char* left(size_t numColumns = 1);
    const char* right(size_t numColumns = 1);
    const char* up(size_t numRows = 1);
    const char* down(size_t numRows = 1);

    /// <remarks>
    /// These do not change the cell position of this object.
    /// </remarks>
    const char* strLeft(size_t numColumns = 1);
    const char* strRight(size_t numColumns = 1);
    const char* strUp(size_t numRows = 1);
    const char* strDown(size_t numRows = 1);

public:
    enum ScanCallbackReturn : int
    {
        CancelWithFailure = -1,
        Continue          = 0,
        CancelWithSuccess = 1
    };

    typedef std::function<ScanCallbackReturn(const char* pStr, size_t column, size_t row)> ScanCallback;

    enum DoWrap : bool
    {
        NoWrap = false,
        Wrap = true
    };

    enum FindBy : bool
    {
        ByColumn = false,
        ByRow = true
    };

    static constexpr const char* FirstNonEmpty = nullptr;

    // All scans and searches proceed only in a forward direction (increasing rows/columns.)

    const char* scanByRow(ScanCallback callback, DoWrap wrap = Wrap, bool includeCurCell = false);
    const char* scanByColumn(ScanCallback callback, DoWrap wrap = Wrap, bool includeCurCell = false);

    /// If pStr is nullptr, this will return on the first non-empty cell.
    const char* findNext(const char* pStr, FindBy findBy = ByRow, DoWrap wrap = Wrap, bool ignoreCase = false, size_t len = 0);

    std::string toString() const;

    static std::string toString(size_t column);
    static std::string toString(size_t column, size_t row);
    static std::string toString(const CsvCell& cell);

private:
    const char* moveToSigned(intmax_t column, intmax_t row);

    bool calcMove(intmax_t& column, intmax_t& row) const;
    bool calcLeft(size_t& numColumns) const;
    bool calcRight(size_t& numColumns) const;
    bool calcUp(size_t& numRows) const;
    bool calcDown(size_t& numRows) const;

private:
    size_t _column = 0;
    size_t _row    = 0;
    const char* _pStr = nullptr;
    const CsvFile& _csv;
};
