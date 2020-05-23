#include "stdafx.h"
#include <sstream>
#include "Database.h"
#include "QGetPath.h"
#include "Preferences.h"

#define DB_APP_ID      0x434f5245

using namespace std;

QSqlDatabase db;

///////////////////////////////////////////////////////////////////////////

size_t DbTransaction::_count = 0;

DbTransaction::DbTransaction()
{
    start();
}

DbTransaction::~DbTransaction()
{
    if (_count > 0)
    {
        if (--_count == 0)
        {
            rollback();
        }
    }
}

void DbTransaction::start()
{
    if (++_count == 1)
    {
        db.transaction();
    }
}

void DbTransaction::rollback()
{
    _count = 0;
    db.rollback();
}

void DbTransaction::commit()
{ 
    _count = 0;
    db.commit();
}

///////////////////////////////////////////////////////////////////////////

QString LibQueryStr(const QString& query)
{
    QSqlQuery sqlQuery(db);
    if (sqlQuery.exec(query))
    {
        // these are separated for debugging purposes
        if (sqlQuery.first())
        {
            return sqlQuery.value(0).toString();
        }
    }
    return QString();
}

QString LibQueryStr(const QString& query, const QString& defaultValue)
{
    QSqlQuery sqlQuery(db);
    if (sqlQuery.exec(query))
    {
        // these are separated for debugging purposes
        if (sqlQuery.first())
        {
            return sqlQuery.value(0).toString();
        }
    }
    return defaultValue;
}

QString LibQueryStr2(const QString& query, const char* pDefaultValue)
{
    QSqlQuery sqlQuery(db);
    if (sqlQuery.exec(query))
    {
        // these are separated for debugging purposes
        if (sqlQuery.first())
        {
            return sqlQuery.value(0).toString();
        }
    }
    return QString(pDefaultValue);
}

int LibQueryInt(const QString& query, int defaultValue)
{
    QSqlQuery sqlQuery(db);
    if (sqlQuery.exec(query))
    {
        // these are separated for debugging purposes
        if (sqlQuery.first())
        {
            return sqlQuery.value(0).toInt();
        }
    }
    return defaultValue;
}

bool LibQueryBool(const QString& query, bool defaultValue)
{
    QSqlQuery sqlQuery(db);
    if (sqlQuery.exec(query))
    {
        // these are separated for debugging purposes
        if (sqlQuery.first())
        {
            return sqlQuery.value(0).toBool();
        }
    }
    return defaultValue;
}

//s = LibraryQuery(QStringLiteral("PRAGMA compile_options;"));
//s = LibraryQuery(QStringLiteral("PRAGMA collation_list;"));
//s = LibraryQuery(QStringLiteral("PRAGMA journal_mode;"));
//s = LibraryQuery(QStringLiteral("PRAGMA locking_mode;"));
//s = LibraryQuery(QStringLiteral("PRAGMA secure_delete;"));
//s = LibraryQuery(QStringLiteral("PRAGMA synchronous;"));
//s = LibraryQuery(QStringLiteral("PRAGMA short_column_names;"));
//s = LibraryQuery(QStringLiteral("PRAGMA fullfsync;"));
//s = LibraryQuery(QStringLiteral("PRAGMA automatic_index;"));
//s = LibraryQuery(QStringLiteral("PRAGMA busy_timeout;"));
//s = LibraryQuery(QStringLiteral("PRAGMA page_size;"));
//s = LibraryQuery(QStringLiteral("PRAGMA cache_size;"));

void DbClose()
{
    db.close();
}

static void AddDatabase()
{
    if (db.driverName() != QStringLiteral("QSQLITE"))
    {
        db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
    }
}

ErrCode DbOpen()
{
    ErrCode errCode = ErrCode::NoError;

    if (!db.isOpen())
    {
        AddDatabase();

        QString dbPathname(QGetDocPath() % QStringLiteral("CoreFit.db"));

        db.setDatabaseName(dbPathname);
        db.setConnectOptions(QStringLiteral("QSQLITE_BUSY_TIMEOUT=15"));
        if (!db.open())
        {
            Log::Error(_WHERE_, nullptr, ErrCode::DbOpenFailed);
            errCode = ErrCode::DbOpenFailed;
        }
        else
        {
            QSqlQuery query;
            query.exec("PRAGMA journal_mode = WAL");
            query.exec(QStringLiteral("PRAGMA locking_mode = EXCLUSIVE"));
            if (!query.exec(QStringLiteral("BEGIN EXCLUSIVE TRANSACTION")))
            {
                DbClose();
                errCode = ErrCode::DbAlreadyOpen;
            }
            else
            {
                query.exec(QStringLiteral("COMMIT"));
                //query.exec(QStringLiteral("PRAGMA case_sensitive_like = TRUE"));

                int appId = LibQueryInt(QStringLiteral("PRAGMA application_id"));
                int userVersion = LibQueryInt(QStringLiteral("PRAGMA user_version"));

                if (appId == 0 && userVersion == 0)
                {
                    errCode = ErrCode::DbCreated;
                }
                else if (appId == DB_APP_ID)
                {
                    // We are not forward compatible with schemas
                    if (userVersion > DB_VERSION)
                    {
                        Log::Format(_WHERE_, nullptr, ErrCode::DbInvalidSchema, "user_version {0} > {1}", to_string(userVersion), TOSTRING(DB_VERSION));
                        DbClose();
                        errCode = ErrCode::DbInvalidSchema;
                    }
                    else
                    {
                        // Check if we need to modify schema
                        switch (userVersion)
                        {
                            case DB_VERSION:
                                break;
                        }
                    }
                }
                else
                {
                    DbClose();

                    ostringstream appIdStr;
                    appIdStr << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << appId;

                    Log::Format(_WHERE_, nullptr, ErrCode::DbInvalidVersion, "application_id was 0x{0}", appIdStr.str());
                    errCode = ErrCode::DbInvalidVersion;
                }
            }
        }
    }

    return errCode;
}

#define EXEC(s) if (!sqlQuery.exec(QStringLiteral(s))) throw DbException(&sqlQuery, ErrCode::DbExecQuery, _WHERE_);

//void _libCreateExtraIndexes()
//{
//    EXEC("CREATE INDEX folderParentsIndex ON folders(parentId)");
//
//    EXEC("CREATE INDEX designsNameIndex     ON designs(name COLLATE NOCASE)");
//    EXEC("CREATE INDEX designsArtistIndex   ON designs(artist COLLATE NOCASE)");
//    EXEC("CREATE INDEX designsFolderIdIndex ON designs(folderId)");
//}
//
//void _libDropExtraIndexes()
//{
//    EXEC("DROP INDEX IF EXISTS folderParentsIndex");
//
//    EXEC("DROP INDEX IF EXISTS designsNameIndex");
//    EXEC("DROP INDEX IF EXISTS designsArtistIndex");
//    EXEC("DROP INDEX IF EXISTS designsFolderIdIndex");
//}

bool DbCreate()
{
    DbTransaction transaction;

    QSqlQuery sqlQuery;

    try
    {
        // Set critical PRAGMAs before any tables are created
        //EXEC("PRAGMA auto_vacuum = 2");
        EXEC("PRAGMA foreign_keys = ON");

        EXEC("CREATE TABLE measures"
             "("
                "measureId "    "INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name "         "VARCHAR(30) NOT NULL,"
                "abbrev "       "VARCHAR(6)"
             ")");

        EXEC("CREATE TABLE conversions"
             "("
                "measureId      INTEGER NOT NULL,"
                "quantity       INTEGER NOT NULL,"
                "ofMeasureId    INTEGER NOT NULL,"
                "FOREIGN KEY(measureId) REFERENCES measures(measureId) ON DELETE CASCADE,"
                "FOREIGN KEY(OfMeasureId) REFERENCES measures(measureId) ON DELETE CASCADE"
             ")");

        EXEC("CREATE TABLE rawIngredients"
             "("
                "ingedientId    INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name           VARCHAR(50) NOT NULL,"
                "measureId      INTEGER NOT NULL,"
                "cost           INTEGER NOT NULL,"
                "description    TEXT,"
                "FOREIGN KEY(measureId) REFERENCES measures(measureId) ON DELETE CASCADE"
             ")");

        EXEC("CREATE TABLE recipes"
             "("
                "recipeId       INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name           VARCHAR(50) NOT NULL,"
                "description    TEXT"
             ")");

        EXEC("CREATE TABLE ingredients"
             "("
                "recipeId       INTEGER NOT NULL,"
                "ingredientId   INTEGER NOT NULL,"
                "FOREIGN KEY(recipeId) REFERENCES recipes(recipeId) ON DELETE CASCADE,"
                "FOREIGN KEY(ingredientId) REFERENCES ingredients(ingredientId) ON DELETE CASCADE"
             ")");

        EXEC("CREATE TABLE customers"
             "("
                "customerId     INTEGER NOT NULL,"
                "firstName      VARCHAR(50),"
                "lastName       VARCHAR(50) NOT NULL"
             ")");

        EXEC("CREATE TABLE orders"
             "("
                "recipeId       INTEGER NOT NULL,"
                "date           VARCHAR(50),"
                "FOREIGN KEY(recipeId) REFERENCES recipes(recipeId) ON DELETE CASCADE"
             ")");

        EXEC("INSERT INTO measures VALUES (1,'Ounce','oz')");
        EXEC("INSERT INTO measures VALUES (2,'Pound','lb')");
        EXEC("INSERT INTO measures VALUES (3,'Teaspoon','tsp')");
        EXEC("INSERT INTO measures VALUES (4,'Tablespoon','Tbsp')");
        EXEC("INSERT INTO measures VALUES (5,'Cup','c')");
        EXEC("INSERT INTO measures VALUES (6,'Pint','pt')");
        EXEC("INSERT INTO measures VALUES (7,'Quart','qt')");
        EXEC("INSERT INTO measures VALUES (8,'Gallon','g')");
        EXEC("INSERT INTO measures VALUES (9,'Slice',NULL)");
        EXEC("INSERT INTO measures VALUES (10,'Pinch',NULL)");

        EXEC("INSERT INTO conversions VALUES (1, 2, 4 )");
        EXEC("INSERT INTO conversions VALUES (2, 16, 1)");
        EXEC("INSERT INTO conversions VALUES (4, 3, 3)");
        EXEC("INSERT INTO conversions VALUES (5, 8, 1)");
        EXEC("INSERT INTO conversions VALUES (5, 16, 4)");
        EXEC("INSERT INTO conversions VALUES (6, 2, 5)");
        EXEC("INSERT INTO conversions VALUES (7, 2, 6)");
        EXEC("INSERT INTO conversions VALUES (8, 4, 7)");

        //48 pinch per tbsp
        //16 pinch per tsp
        //5 tablespoons + 1 teaspoon = 1/3 cup
        
        // Add version & id. Without these, the datase will be considered invalid.

        EXEC("PRAGMA user_version = " TOSTRING(DB_VERSION));
        EXEC("PRAGMA application_id = " TOSTRING(DB_APP_ID));

        //_libCreateExtraIndexes();

        transaction.commit();
    }
    catch (const DbException& e)
    {
        e.Log("LibCreate");
        DbClose();
        QFile::remove(db.databaseName());
        return false;
    }

    return true;
}

/////////////////////////////////////////////////////////////////////////////

ErrCode DbCheck()
{
    ErrCode errCode = ErrCode::NoError;

    if (!db.isOpen())
    {
        Log::Error(_WHERE_, nullptr, ErrCode::DbNotOpen);
        errCode = ErrCode::DbNotOpen;
    }
    else
    {
        try
        {
            QSqlQuery sqlQuery;
            EXEC("PRAGMA foreign_key_check");
            EXEC("PRAGMA integrity_check");

            Log::Info(_WHERE_, "Finished DbCheck");
        }
        catch (const DbException& e)
        {
            //out << "Critical database error prevented or halted processing." << endl;
            e.Log(nullptr);
            errCode = e.code;
        }
    }

    return errCode;
}

ErrCode DbVacuum()
{
    ErrCode errCode = ErrCode::NoError;

    if (!db.isOpen())
    {
        Log::Error(_WHERE_, nullptr, ErrCode::DbNotOpen);
        errCode = ErrCode::DbNotOpen;
    }
    else
    {
        try
        {
            QSqlQuery sqlQuery;
            EXEC("VACUUM");
            EXEC("REINDEX");

            Log::Info(_WHERE_, "Finished DbVacuum");
        }
        catch (const DbException& e)
        {
            e.Log(nullptr);
            errCode = e.code;
        }
    }

    return errCode;
}

/////////////////////////////////////////////////////////////////////////////

DbQuery::DbQuery()
    : QSqlQuery(db)
{
}

DbQuery::DbQuery(const QString& query, const char* pFile, const char* pFunction, int line, const char* pInfo)
    : QSqlQuery(db)
{
    if (!QSqlQuery::exec(query))
    {
        throw DbException(this, ErrCode::DbExecQuery, pInfo, pFile, pFunction, line);
    }
}

bool DbQuery::exec(const QString& query, const char* pFile, const char* pFunction, int line, const char* pInfo)
{
    bool result = true;
    if (!QSqlQuery::exec(query))
    {
        Log::String(pFile, pFunction, line, nullptr, ErrCode::DbExecQuery, DbCreateErrorStr(this), pInfo ? pInfo : "");
        result = false;
    }
    return result;
}

bool DbQuery::exec(const QString& query, const char* pFile, const char* pFunction, int line, const InfoCallback& infoCallback)
{
    bool result = true;
    if (!QSqlQuery::exec(query))
    {
        Log::String(pFile, pFunction, line, nullptr, ErrCode::DbExecQuery, DbCreateErrorStr(this), infoCallback().c_str() );
        result = false;
    }
    return result;
}

///////////////////////////////////////////////////////////////////////////

DbPreparedQuery::DbPreparedQuery()
    : QSqlQuery(db)
{
}

DbPreparedQuery::DbPreparedQuery(const QString& query, const char* pFile, const char* pFunction, int line, const char* pInfo)
    : QSqlQuery(db)
{
    if (!QSqlQuery::prepare(query))
    {
        throw DbException(this, ErrCode::DbPrepareQuery, pInfo, pFile, pFunction, line);
    }
}

bool DbPreparedQuery::prepare(const QString& query, const char* pFile, const char* pFunction, int line, const char* pInfo)
{
    bool result = true;
    if (!QSqlQuery::prepare(query))
    {
        Log::String(pFile, pFunction, line, nullptr, ErrCode::DbPrepareQuery, DbCreateErrorStr(this), pInfo ? pInfo : "");
        result = false;
    }
    return result;
}

bool DbPreparedQuery::exec(const char* pFile, const char* pFunction, int line, const char* pInfo)
{
    bool result = true;
    if (!QSqlQuery::exec())
    {
        Log::String(pFile, pFunction, line, nullptr, ErrCode::DbExecQuery, DbCreateErrorStr(this), pInfo ? pInfo : "");
        result = false;
    }
    return result;
}

