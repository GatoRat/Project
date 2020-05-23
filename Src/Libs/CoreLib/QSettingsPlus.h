// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

class QSettingsPlus : public QSettings
{
public:
    QSettingsPlus(QObject* pParent = nullptr) : QSettings(pParent) {}

    QMap<QString, QVariant> GetKeyValueMap(const QString& prefix);
    QVector<QPair<QString, QVariant>> GetKeyValueVector(const QString& prefix);
};