// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

class Arguments
{
public:
    explicit Arguments(QStringList args);

    bool HasNext() const;
    bool Next();

    bool IsNextSwitch() const;
    char GetSwitch();

    QString GetCurArg() const;
    QString GetCurArgString() const;
    QStringRef GetCurArgStringRef() const;
    int GetCurArgValue(int defaultValue = 0);

    bool GetBool(bool defaultVal, bool strict = false);
    bool GetValue(int& value, int defaultValue, bool mustHaveValue = false);
    QStringList GetRemainingArgs() const;

protected:
    QStringList _args;
    int         _curArg = 0;
    int         _curArgOffset = 0;

    bool NextIfNumber();
};
