// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "Util/Detect/DetectCompiler.h"
#include "Util/Log/Log.h"

enum class ErrCode : unsigned
{
    Base = 0,
        Success = Base,
        NoError = Base,
        Assert,
        Debug,
        QtMessage,
        NothingToDo,
        UserKeyRead,
        UserKeyWrite,

    Data = 100,
        DataInvalid = Data,

    Read = 200,
        ReadOpenFailed = Read,
        ReadMissingFile,
        ReadInvalidFile,
        ReadFailed,

    Write = 300,
        WriteCreateFailed = Write,
        WriteCommitFailed,
        ReportCreateFailed,

    Db = 400,
        DbCreated = Db,
        DbOpenFailed,
        DbCorrupt,
        DbAlreadyOpen,
        DbCreateFailed,
        DbNotOpen,
        DbInvalidVersion,
        DbInvalidSchema,
        DbPrepareQuery,
        DbExecQuery,
        DbFailedCheck,
        DbFailedForeignKeyCheck,
        DbFailedIntegrityCheck,
};

const char* GetErrCodeString (ErrCode code);
QString     GetErrCodeQString(ErrCode code);

///////////////////////////////////////////////////////////////////////////

namespace Log
{
    void Info(const char* pFile, const char* pFunction, int line, const char* pCause);

    void Error(const char* pFile, const char* pFunction, int line, const char* pCause, ErrCode code);

    void String(const char* pFile, const char* pFunction, int line, const char* pCause,
                ErrCode code, std::string_view string);
    void String(const char* pFile, const char* pFunction, int line, const char* pCause,
                ErrCode code, std::string_view string1, std::string_view string2);

    //void String(const char* pFile, const char* pFunction, int line, const char* pCause,
    //            ErrCode code, const QString& string1);
    //void String(const char* pFile, const char* pFunction, int line, const char* pCause,
    //            ErrCode code, const QString& string1, const QString& string2);

    void Format(const char* pFile, const char* pFunction, int line, const char* pCause,
                ErrCode code, std::string_view formatString,
                std::string_view string0);
    void Format(const char* pFile, const char* pFunction, int line, const char* pCause,
                ErrCode code, std::string_view formatString,
                std::string_view string0, std::string_view string1);
    void Format(const char* pFile, const char* pFunction, int line, const char* pCause,
                ErrCode code, std::string_view formatString,
                std::string_view string0, std::string_view string1, std::string_view string2);

    //void Format(const char* pFile, const char* pFunction, int line, const char* pCause,
    //            ErrCode code, const QString& formatString,
    //            const QString& string0);
    //void Format(const char* pFile, const char* pFunction, int line, const char* pCause,
    //            ErrCode code, const QString& formatString,
    //            const QString& string0, const QString& string1);
    //void Format(const char* pFile, const char* pFunction, int line, const char* pCause,
    //            ErrCode code, const QString& formatString,
    //            const QString& string0, const QString& string1, const QString& string2);

    void Data(const char* pFile, const char* pFunction, int line, const char* pCause,
              ErrCode code, std::string_view string, const void* pData, size_t dataLen);

    //ErrCode DbErr(const char* pFile, const char* pFunction, int line, const char* pCause, ErrCode code);
    //ErrCode DbErr(const char* pFile, const char* pFunction, int line, const char* pCause, ErrCode code, std::string_view string1);
    //ErrCode DbErr(const char* pFile, const char* pFunction, int line, const char* pCause, ErrCode code, std::string_view string1, std::string_view string2);

    ErrCode DbErr(const char* pFile, const char* pFunction, int line, const char* pCause,
                  ErrCode code, const QSqlQuery* pQuery, const char* pString = nullptr);
}

std::string FormatErrorStr(const char* pFile, const char* pFunction, int line, const char* pCause, ErrCode code);
std::string DbCreateErrorStr(const QSqlQuery* pQuery);

///////////////////////////////////////////////////////////////////////////

struct Exception
{
    const char* pCause;
    const char* pFile;
    const char* pFunction;
    int         line;
    ErrCode     code;

    Exception(ErrCode code, const char* pCause, const char* pFile, const char* pFunction, int line);
    Exception(ErrCode code, const char* pFile, const char* pFunction, int line);
    virtual ~Exception() = default;

    virtual void Log(const char* pCatchInfo) const;
    void LogString(std::string_view string) const;
};

struct FileException : public Exception
{
    QString pathname;
    QFileDevice::FileError fileError;

    FileException(const QString& pathname, QFileDevice::FileError fileError, ErrCode code, const char* pCause, const char* pFile, const char* pFunction, int line);
    FileException(const QString& pathname, QFileDevice::FileError fileError, ErrCode code, const char* pFile, const char* pFunction, int line);
    FileException(const QFileDevice& file, ErrCode code, const char* pFile, const char* pFunction, int line);
    FileException(const QDataStream& stream, ErrCode code, const char* pFile, const char* pFunction, int line);

    void Log(const char* pCatchInfo) const override;
};

struct DbException : public Exception
{
    std::string queryErrStr;

    DbException(const QSqlQuery* pQuery, ErrCode code, const char* pCause, const char* pFile, const char* pFunction, int line);
    DbException(const QSqlQuery* pQuery, ErrCode code, const char* pFile, const char* pFunction, int line);

    void Log(const char* pCatchInfo) const override;
};
