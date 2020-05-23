// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "Arguments.h"
#include "Util/String/ConvertW.h"

Arguments::Arguments(QStringList args)
    : _args(args)
{
}

bool Arguments::HasNext() const
{ 
    return _curArg + 1 < _args.length();
}

bool Arguments::Next()
{
    if (_curArg + 1 >= _args.length())
    {
        return false;
    }

    ++_curArg;
    _curArgOffset = 0;
    return true;
}

bool Arguments::IsNextSwitch() const
{
    return HasNext() && _args[_curArg + 1][0].toLatin1() == '-' && _args[_curArg + 1].length() > 1;
}

char Arguments::GetSwitch()
{
    if (_args[_curArg][0].toLatin1() == '-' && _args[_curArg].length() > 1)
    {
        _curArgOffset = 2;
        return _args[_curArg][1].toLatin1();
    }
    return 0;
}

QString Arguments::GetCurArg() const
{
    return _args[_curArg];
}

QString Arguments::GetCurArgString() const
{
    return _args[_curArg].mid(_curArgOffset);
}

QStringRef Arguments::GetCurArgStringRef() const
{
    return _args[_curArg].midRef(_curArgOffset);
}

int Arguments::GetCurArgValue(int defaultValue)
{
    int value;
    GetValue(value, defaultValue);
    return value;
}

bool Arguments::GetBool(bool defaultVal, bool strict)
{
    if (_curArgOffset >= _args[_curArg].length())
    {
        return defaultVal;
    }

    char ch = _args[_curArg][_curArgOffset].toLatin1(); //-V108

    if (strict)
    {
        if (ch != '-' && ch != '+')
        {
            return defaultVal;
        }
    }

    ++_curArgOffset;
    return ch != '-';
}

bool Arguments::GetValue(int& value, int defaultValue, bool mustHaveValue)
{
    if (_curArgOffset < _args[_curArg].length() || NextIfNumber())
    {
        //val = ToUint64(pStr, err, pEnd, UINT64_MAX, allowCommas);
        //success = err >= ToUint64_Success;

        //ConvertW::ToUint64Err err;
        //int curArgOffset = _curArgOffset;

        //uint64_t ToUint64(const wchar_t* pStr, err, &curArgOffset);

        //value = ConvertW::ToUint64(_args[_curArg], &curArgOffset, &err);
        //if (err != StrToIntErr::Success)
        //{
        //    value = defaultValue;
        //    return false;
        //}
        //_curArgOffset = curArgOffset;
        return true;
    }

    value = defaultValue;
    return mustHaveValue ? false : true;
}

QStringList Arguments::GetRemainingArgs() const
{
    QStringList remainder;
    for (int i = _curArg; _args.length() < i; i++)
    {
        remainder.append(_args[i]);
    }
    return remainder;
}

bool Arguments::NextIfNumber()
{
    if (HasNext() && !IsNextSwitch() && _args[_curArg + 1][0].isDigit())
    {
        ++_curArg;
        _curArgOffset = 0;
        return true;
    }
    return false;
}
