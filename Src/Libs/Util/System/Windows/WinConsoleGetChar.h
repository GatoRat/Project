// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <utility>
#include "WinHandle.h"

char WinConsoleGetChar(WinEvent& shutdownEvent);

///////////////////////////////////////////////////////////////////////////

// There can be only one of these and it doesn't bother ensuring that

class WinConsoleCtrlHandler : public WinEvent
{
public:
    WinConsoleCtrlHandler();
    explicit WinConsoleCtrlHandler(WinEvent& shutdownEvent);
    ~WinConsoleCtrlHandler();

    static WinEvent _appClosed;

private:
    void init();
};
