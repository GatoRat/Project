// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "QSettingsPlus.h"

QMap<QString, QVariant> QSettingsPlus::GetKeyValueMap(const QString& prefix)
{
    QMap<QString, QVariant> settingsMap;
    beginGroup(prefix);

    QStringList keys = childKeys();
    for (const QString& key : keys)
        settingsMap[key] = value(key);

    endGroup();
    return settingsMap;
}

QVector<QPair<QString, QVariant>> QSettingsPlus::GetKeyValueVector(const QString& prefix)
{
    QVector<QPair<QString, QVariant>> settingsArray;
    beginGroup(prefix);

    QStringList keys = childKeys();
    settingsArray.resize(keys.size());
    for (int i = 0; i < keys.size(); i++)
    {
        settingsArray[i].first = keys[i];
        settingsArray[i].second = value(settingsArray[i].first);
    }

    endGroup();
    return settingsArray;
}
