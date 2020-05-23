// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "QGetPath.h"
#include "Util/System/SysGetPath.h"

static QString appPath;
static QString docPath;
static QString downloadPath;
static QString tmpPath;

static QString appDataPath;
static QString roamingDataPath;
static QString programDataPath;

bool InitDirectories(QStringList& /*arguments*/, const char* pDocSubdir)
{
    appPath         = QString::fromWCharArray(SysGetPath::App().wstring().c_str()) % QChar('/');
    docPath         = QString::fromWCharArray(SysGetPath::Doc().wstring().c_str()) % QChar('/');
    if (pDocSubdir && *pDocSubdir)
    {
        docPath += pDocSubdir;
        QDir().mkdir(docPath);
        docPath += QChar('/');
    }

    downloadPath    = QString::fromWCharArray(SysGetPath::Download().wstring().c_str()) % QChar('/');
    tmpPath         = QString::fromWCharArray(SysGetPath::Tmp().wstring().c_str()) % QChar('/');

    appDataPath     = QString::fromWCharArray(SysGetPath::AppData().wstring().c_str()) % QChar('/');;
    roamingDataPath = QString::fromWCharArray(SysGetPath::RoamingData().wstring().c_str()) % QChar('/');;
    programDataPath = QString::fromWCharArray(SysGetPath::ProgramData().wstring().c_str()) % QChar('/');;

    return true;
}

QString QGetAppPath()
{
    return appPath;
}

QString QGetDocPath()
{
    return docPath;
}

QString QGetDownloadPath()
{
    return downloadPath;
}

QString QGetTmpPath()
{
    return tmpPath;
}

QString QGetAppDataPath()
{
    return appDataPath;
}

QString QGetRoamingDataPath()
{
    return roamingDataPath;
}

QString QGetProgramDataPath()
{
    return programDataPath;
}

bool DoesFileExist(const QString& pathname)
{
    QFile file(pathname);
    return file.exists();
}
