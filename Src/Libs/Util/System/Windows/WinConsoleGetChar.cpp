// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "WinConsoleGetChar.h"
#include <iostream>
#include "Util/String/ASCII.h"
#include "Util/Time/Defines.Time.h"

using namespace std;

char WinConsoleGetChar(WinEvent& shutdownEvent)
{
    HANDLE events[2];

    events[0] = shutdownEvent;
    events[1] = GetStdHandle(STD_INPUT_HANDLE);               // Get a Windows handle to the keyboard input

    DWORD consoleMode;
    GetConsoleMode(events[1], &consoleMode);
    SetConsoleMode(events[1], 0);

    char ch = -1;

    while (WaitForMultipleObjects(2, events, false, INFINITE) == WAIT_OBJECT_0 + 1)
    {
        DWORD read;
        INPUT_RECORD input;
        if (ReadConsoleInput(events[1], &input, 1, &read) && read == 1)
        {
            if (input.EventType == KEY_EVENT && input.Event.KeyEvent.bKeyDown)
            {
                if (input.Event.KeyEvent.uChar.AsciiChar >= ASCII::Space &&
                    input.Event.KeyEvent.uChar.AsciiChar < 127)
                {
                    ch = input.Event.KeyEvent.uChar.AsciiChar;
                    break;
                }
                else if (input.Event.KeyEvent.uChar.AsciiChar == ASCII::CR ||
                         input.Event.KeyEvent.uChar.AsciiChar == ASCII::LF)
                {
                    cout << endl;
                }
                else if (input.Event.KeyEvent.uChar.AsciiChar == ASCII::ETX ||
                         input.Event.KeyEvent.uChar.AsciiChar == ASCII::ESC)
                {
                    shutdownEvent.Set();
                }
            }
        }
        else
        {
            shutdownEvent.Set();
        }
    }

    SetConsoleMode(events[1], consoleMode);

    return shutdownEvent.IsSet() ? -1 : ch;
}

///////////////////////////////////////////////////////////////////////////

static WinEvent* pShutdownEvent = nullptr;
WinEvent WinConsoleCtrlHandler::_appClosed(TRUE);

static BOOL WINAPI HandlerRoutine(DWORD /*ctrlType*/)
{
    if (pShutdownEvent)
    {
        pShutdownEvent->Set();
    }
    WinConsoleCtrlHandler::_appClosed.WaitFor(10 * MILLISECONDS_PER_SECOND);
    return TRUE;
}

WinConsoleCtrlHandler::WinConsoleCtrlHandler()
    : WinEvent(TRUE)
{
    init();
}

WinConsoleCtrlHandler::WinConsoleCtrlHandler(WinEvent& shutdownEvent)
    : WinEvent(shutdownEvent, TRUE)
{
    init();
}

WinConsoleCtrlHandler::~WinConsoleCtrlHandler()
{
    // DO NOT reset CtrlHandler; it must stay active for the app close to work.
    pShutdownEvent = nullptr;
}

void WinConsoleCtrlHandler::init()
{
    pShutdownEvent = this;
    SetConsoleCtrlHandler(HandlerRoutine, TRUE);
}
