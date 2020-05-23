// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

bool InitDirectories(QStringList& arguments, const char* pDocSubdir = nullptr);

QString QGetAppPath();
QString QGetDocPath();
QString QGetDownloadPath();
QString QGetTmpPath();

QString QGetAppDataPath();
QString QGetRoamingDataPath();
QString QGetProgramDataPath();

bool DoesFileExist(const QString& pathname);