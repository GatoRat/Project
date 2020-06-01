// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "../ErrCode.h"

#define DB_VERSION   1

class PreparedQuery;

/////////////////////////////////////////////////////////////////////////////

extern QSqlDatabase db;

ErrCode DbOpen();
bool    DbCreate();
void    DbClose();

ErrCode DbCheck();
ErrCode DbVacuum();

/////////////////////////////////////////////////////////////////////////////

class DbTransaction
{
public:
    DbTransaction();
    ~DbTransaction();

    void start();
    void rollback();
    void commit();

private:
    static size_t _count;
};

/////////////////////////////////////////////////////////////////////////////

QString LibQueryStr (const QString& query);
QString LibQueryStr (const QString& query, const QString& defaultValue);
QString LibQueryStr2(const QString& query, const char* pDefaultValue);
int     LibQueryInt (const QString& query, int defaultValue = 0);
bool    LibQueryBool(const QString& query, bool defaultValue = false);

/////////////////////////////////////////////////////////////////////////////

class DbQuery : public QSqlQuery
{
public:
    typedef std::function<std::string()> InfoCallback;

public:
    DbQuery();
    DbQuery(const QString& query, const char* pFile, const char* pFunction, int line, const char* pInfo = nullptr);

    // These hide base exec() by design
    bool exec(const QString& query, const char* pFile, const char* pFunction, int line, const char* pInfo = nullptr);
    bool exec(const QString& query, const char* pFile, const char* pFunction, int line, const InfoCallback& infoCallback);
};

class DbPreparedQuery : public QSqlQuery
{
public:
    DbPreparedQuery();
    DbPreparedQuery(const QString& query, const char* pFile, const char* pFunction, int line, const char* pInfo = nullptr);

    bool prepare(const QString& query, const char* pFile, const char* pFunction, int line, const char* pInfo = nullptr);
    bool exec(const char* pFile, const char* pFunction, int line, const char* pInfo = nullptr);
};