// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include <string>
#include "Util/String/Convert.h"
#include "Util/String/Str.h"
#include "ErrCode.h"

using namespace std;

static map<ErrCode, const char*> errCodeStrings
{
    { ErrCode::NoError           , QT_TR_NOOP("No Error"            )},
    { ErrCode::Assert            , QT_TR_NOOP("Assert"              )},
    { ErrCode::Debug             , QT_TR_NOOP("Debug"               )},
    { ErrCode::QtMessage         , QT_TR_NOOP("Qt message"          )},
    { ErrCode::NothingToDo       , QT_TR_NOOP("Nothing to do"       )},
    { ErrCode::UserKeyRead       , QT_TR_NOOP("UserKeyRead"         )},
    { ErrCode::UserKeyWrite      , QT_TR_NOOP("UserKeyWrite"        )},

    { ErrCode::DataInvalid       , QT_TR_NOOP("Invalid data"        )},

    { ErrCode::ReadOpenFailed    , QT_TR_NOOP("Read open failed"    )},
    { ErrCode::ReadMissingFile   , QT_TR_NOOP("Read missing file"   )},
    { ErrCode::ReadInvalidFile   , QT_TR_NOOP("Read invalid file"   )},
    { ErrCode::ReadFailed        , QT_TR_NOOP("Read failed"         )},

    { ErrCode::WriteCreateFailed , QT_TR_NOOP("Write create failed" )},
    { ErrCode::WriteCommitFailed , QT_TR_NOOP("Write commit failed" )},
    { ErrCode::ReportCreateFailed, QT_TR_NOOP("Report create failed")},

    { ErrCode::DbCreated         , QT_TR_NOOP("Db created"          )},
    { ErrCode::DbOpenFailed      , QT_TR_NOOP("Db open failed"      )},
    { ErrCode::DbCorrupt         , QT_TR_NOOP("Db corrupt"          )},
    { ErrCode::DbAlreadyOpen     , QT_TR_NOOP("Db already open"     )},
    { ErrCode::DbCreateFailed    , QT_TR_NOOP("Db create failed"    )},
    { ErrCode::DbNotOpen         , QT_TR_NOOP("Db not open"         )},
    { ErrCode::DbInvalidVersion  , QT_TR_NOOP("Db invalid version"  )},
    { ErrCode::DbInvalidSchema   , QT_TR_NOOP("Db invalid schema"   )},
    { ErrCode::DbPrepareQuery    , QT_TR_NOOP("Db prepare query"    )},
    { ErrCode::DbExecQuery       , QT_TR_NOOP("Db exec query"       )},
    { ErrCode::DbFailedCheck     , QT_TR_NOOP("Db failed check"     )},
};

const char* GetErrCodeString(ErrCode code)
{
    auto it = errCodeStrings.find(code);
    return it == errCodeStrings.end() ? QT_TR_NOOP("Unknown") : it->second;
}

QString GetErrCodeQString(ErrCode code)
{
    const char* pErrStr = GetErrCodeString(code);
    return QCoreApplication::tr(pErrStr);
}

///////////////////////////////////////////////////////////////////////////

extern QSqlDatabase db;

namespace Log
{
    void Info(const char* pFile, const char* pFunction, int line, const char* pCause)
    {
        String(pFile, pFunction, line, pCause, ErrCode::NoError, string_view());
    }

    void Error(const char* pFile, const char* pFunction, int line, const char* pCause, ErrCode code)
    {
        String(pFile, pFunction, line, pCause, code, string_view(), string_view());
    }

    void String(const char* pFile, const char* pFunction, int line, const char* pCause,
                ErrCode code, string_view string1)
    {
        String(pFile, pFunction, line, pCause, code, string1, string_view());
    }

    void String(const char* pFile, const char* pFunction, int line, const char* pCause,
                ErrCode code, string_view string1, string_view string2)
    {
        string errStr = FormatErrorStr(pFile, pFunction, line, pCause, code);
        if (!string1.empty())
        {
            errStr += ": ";
            errStr += string1;
        }
        Log::Strings(errStr, string2);
    }

    void Format(const char* pFile, const char* pFunction, int line, const char* pCause,
                ErrCode code, string_view formatString,
                string_view string0)
    {
        Format(pFile, pFunction, line, pCause, code, formatString, string0, string_view(), string_view());
    }

    void Format(const char* pFile, const char* pFunction, int line, const char* pCause,
                ErrCode code, string_view formatString,
                string_view string0, string_view string1)
    {
        Format(pFile, pFunction, line, pCause, code, formatString, string0, string1, string_view());
    }

    void Format(const char* pFile, const char* pFunction, int line, const char* pCause,
                ErrCode code, string_view formatString,
                string_view string0, string_view string1, string_view string2)
    {
        string errStr = FormatErrorStr(pFile, pFunction, line, pCause, code);
        if (!formatString.empty())
        {
            errStr += ": ";
            errStr += Str::Format3(formatString, string0, string1, string2);
        }
        Log::Format(errStr, string0, string1, string2);
    }

    void Data(const char* pFile, const char* pFunction, int line, const char* pCause,
              ErrCode code, string_view string0, const void* pData, size_t dataLen)
    {
        string errStr = FormatErrorStr(pFile, pFunction, line, pCause, code);
        if (!string0.empty())
        {
            errStr += ": ";
            errStr += string0;
        }
        Log::Data(errStr, pData, dataLen);
    }

    ErrCode DbErr(const char* pFile, const char* pFunction, int line, const char* pCause,
                  ErrCode code, const QSqlQuery* pQuery, const char* pString)
    {
        string errStr(Str::IsNull(pString) ? "" : pString);
        errStr += DbCreateErrorStr(pQuery);

        String(pFile, pFunction, line, pCause, code, errStr);

        return code;
    }
}

string FormatErrorStr(const char* pFile, const char* pFunction, int line, const char* pCause, ErrCode code)
{
    if (pFile)
    {
#ifdef Q_CC_MSVC
        const char* p = strrchr(pFile, '\\');
#else
        const char* p = strrchr(pFile, '/');
#endif
        if (p)
        {
            pFile = ++p;
        }
    }

    // "cause{code}: file(line)[function]: "
    // "read: read.cpp(56)[ReadFile(char* pFile)]: "
    // "read: [ReadFile(char* pFile)]: "
    // "read: read.cpp(56): "

    string errStr;
    errStr.reserve(256);

    if (!Str::IsEmptyOrWhiteSpace(pCause))
    {
        errStr += pCause;
    }
    else if (code != ErrCode::NoError)
    {
        errStr += GetErrCodeString(code);
    }

    if (code != ErrCode::NoError)
    {
        errStr += '(';
        errStr += to_string(static_cast<int>(code));
        errStr += ')';
    }

    if (!Str::IsEmptyOrWhiteSpace(pFile))
    {
        errStr += ": ";
        errStr += pFile;
        if (const size_t offset = errStr.rfind('.'); offset != string::npos)
        {
            errStr.resize(offset);
        }

        errStr += '(';
        errStr += to_string(line);
        errStr += ')';

        if (!Str::IsNullOrWhiteSpace(pFunction))
        {
            errStr += '[';
            errStr += pFunction;
            errStr += ']';
        }
    }

    return errStr;
}

string DbCreateErrorStr(const QSqlQuery* pQuery)
{
    QString lastQuery;
    QString driverText;
    QString databaseText;

    if (pQuery)
    {
        lastQuery    = pQuery->lastQuery();
        driverText   = pQuery->lastError().driverText();
        databaseText = pQuery->lastError().databaseText();
    }
    else
    {
        driverText   = db.lastError().driverText();
        databaseText = db.lastError().databaseText();
    }

    QString errStr;

    if (!driverText.isEmpty())
    {
        errStr += driverText;
    }

    if (!databaseText.isEmpty())
    {
        if (!errStr.isEmpty())
        {
            errStr += QStringLiteral(": ");
        }
        errStr += databaseText;
    }

    if (!lastQuery.isEmpty())
    {
        if (!errStr.isEmpty())
        {
            errStr += QStringLiteral(": ");
        }
        errStr += lastQuery;
    }

    return errStr.toUtf8().toStdString();
}

///////////////////////////////////////////////////////////////////////////

Exception::Exception(ErrCode code_, const char* pCause_, const char* pFile_, const char* pFunction_, int line_)
    : pCause   { pCause_    }
    , pFile    { pFile_     }
    , pFunction{ pFunction_ }
    , line     { line_      }
    , code     { code_      }
{
}

Exception::Exception(ErrCode code_, const char* pFile_, const char* pFunction_, int line_)
    : pCause   { nullptr    }
    , pFile    { pFile_     }
    , pFunction{ pFunction_ }
    , line     { line_      }
    , code     { code_      }
{
}

void Exception::Log(const char* pCatchInfo) const
{
    Log::String(pFile, pFunction, line, pCause, code, pCatchInfo ? pCatchInfo : "");
}

void Exception::LogString(string_view string) const
{
    Log::String(pFile, pFunction, line, pCause, code, string);
}

///////////////////////////////////////////////////////////////////////////

FileException::FileException(const QString& pathname_, QFileDevice::FileError fileError_, ErrCode code,
    const char* pCause, const char* pFile, const char* pFunction, int line)
    : Exception(code, pCause, pFile, pFunction, line)
    , pathname(pathname_)
    , fileError(fileError_)
{
}

FileException::FileException(const QString& pathname_, QFileDevice::FileError fileError_, ErrCode code,
    const char* pFile, const char* pFunction, int line)
    : Exception(code, pFile, pFunction, line)
    , pathname(pathname_)
    , fileError(fileError_)
{
}

FileException::FileException(const QFileDevice& file, ErrCode code, const char* pFile, const char* pFunction, int line)
    : Exception(code, pFile, pFunction, line)
    , pathname(file.fileName())
    , fileError(file.error())
{
}

FileException::FileException(const QDataStream& stream, ErrCode code, const char* pFile, const char* pFunction, int line)
    : Exception(code, pFile, pFunction, line)
{
    QFileDevice* pFileDevice = qobject_cast<QFileDevice*>(stream.device());
    if (pFileDevice)
    {
        pathname  = pFileDevice->fileName();
        fileError = pFileDevice->error();
    }
}

void FileException::Log(const char* pCatchInfo) const
{
    string str;
    str.reserve(pathname.size() + Str::GetLen(pCatchInfo) + 48);

    if (!Str::IsNullOrEmpty(pCatchInfo))
    {
        str += pCatchInfo;
        str += ':';
        str += ' ';
    }

    str += "file error (";
    str += to_string(static_cast<int>(fileError));
    str += ')';

    if (pathname.isEmpty())
    {
        str += "<empty>: ";
    }
    else
    {
        str += '"';
        str += pathname.toUtf8().constData();
        str += "\": ";
    }

    Exception::LogString(str);
}

///////////////////////////////////////////////////////////////////////////

DbException::DbException(const QSqlQuery* pQuery, ErrCode code, const char* pCause, const char* pFile, const char* pFunction, int line)
    : Exception(code, pCause, pFile, pFunction, line)
    , queryErrStr(DbCreateErrorStr(pQuery))
{
}

DbException::DbException(const QSqlQuery* pQuery, ErrCode code, const char* pFile, const char* pFunction, int line)
    : Exception(code, pFile, pFunction, line)
    , queryErrStr(DbCreateErrorStr(pQuery))
{
}

void DbException::Log(const char* pCatchInfo) const
{
    Log::String(pFile, pFunction, line, pCause, code, queryErrStr, Convert::ToStrView(pCatchInfo));
}
