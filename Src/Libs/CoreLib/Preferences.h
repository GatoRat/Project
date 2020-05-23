// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "QSettingsPlus.h"

extern QSettingsPlus* pSettings;

enum class TextColor
{
    Black,
    Red,
    Grey
};

struct Preferences
{
    struct General
    {
        TextColor textColor;

        General();
        void Load();
        void Save(const General& general);

    } general;

    struct Library
    {
        bool useStartsWith;

        Library();
        void Load();
        void Save(const Library& library);

    } library;

public:
    Preferences();
    ~Preferences();

    void Load();
};

extern Preferences preferences;

struct PreferencesInit
{
    PreferencesInit()               { pSettings = new QSettingsPlus; preferences.Load(); }
    ~PreferencesInit()              { delete pSettings; pSettings = nullptr; }
};
