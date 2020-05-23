// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "Preferences.h"

QSettingsPlus* pSettings = nullptr;
Preferences preferences;

Preferences::Preferences()
{
}

Preferences::~Preferences()
{
}

void Preferences::Load()
{
    general.Load();
    library.Load();
}

///////////////////////////////

static QString settingsPath_general = QStringLiteral("general");
static QString settingsPath_fillSolid = QStringLiteral("textColor");

Preferences::General::General()
    : textColor(TextColor::Red)
{
}

void Preferences::General::Load()
{
    if (pSettings)
    {
        pSettings->beginGroup(settingsPath_general);

        textColor = (TextColor)pSettings->value(settingsPath_fillSolid, true).toInt();
        if (textColor != TextColor::Black && textColor != TextColor::Grey && textColor != TextColor::Red)
        {
            textColor = TextColor::Red;
        }

        pSettings->endGroup();
    }
}

void Preferences::General::Save(const General& general)
{
    if (pSettings)
    {
        pSettings->beginGroup(settingsPath_general);

        if (textColor != general.textColor)
        {
            textColor = general.textColor;
            pSettings->setValue(settingsPath_fillSolid, (int)textColor);
        }

        pSettings->endGroup();
    }
}

///////////////////////////////

static QString settingsPath_library = QStringLiteral("library");
static QString settingsPath_useStartsWith = QStringLiteral("useStartsWith");

Preferences::Library::Library()
    : useStartsWith(true)
{
}

void Preferences::Library::Load()
{
    if (pSettings)
    {
        pSettings->beginGroup(settingsPath_library);

        useStartsWith = pSettings->value(settingsPath_useStartsWith, true).toBool();

        pSettings->endGroup();
    }
}

void Preferences::Library::Save(const Library& library)
{
    if (pSettings)
    {
        pSettings->beginGroup(settingsPath_library);

        if (useStartsWith != library.useStartsWith)
        {
            useStartsWith = library.useStartsWith;
            pSettings->setValue(settingsPath_useStartsWith, useStartsWith);
        }

        pSettings->endGroup();
    }
}
