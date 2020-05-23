// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#ifdef _WIN32

    #include "../Detect/DetectCompiler.h"
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif // WIN32_LEAN_AND_MEAN
    #define NOATOM
    #define NOCLIPBOARD
    #define NOCOLOR
    #define NOCOMM
    #define NODEFERWINDOWPOS
    #define NODRAWTEXT
    #ifndef WIN_USE_GDI
        #define NOGDI
    #endif
    #define NOGDICAPMASKS
    #define NOHELP
    #define NOICONS
    #define NOIME
    #define NOKANJI
    #define NOKEYSTATES
    //#define NOMB // MessageBox
    #define NOMCX
    #define NOMEMMGR
    #define NOMENUS
    #define NOMETAFILE
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif // NOMINMAX
    #define NOOPENFILE
    #define NOPROFILER
    #define NORASTEROPS
    #define NOSCROLL
    #ifndef WIN_USE_SERVICE
        #define NOSERVICE
    #endif
    #define NOSOUND
    #define NOSYSCOMMANDS
    #define NOSYSMETRICS
    #define NOTEXTMETRIC
    #define NOVIRTUALKEYCODES
    #define NOWH
    #define NOWINOFFSETS
    #define NOWINSTYLES
    #define OEMRESOURCE
    #ifndef STRICT
        #define STRICT             1
    #endif

    #include <WinSDKVer.h>
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0601 // Windows 7
        //#define _WIN32_WINNT 0x0A00 // Windows 10: Mostly newer XAudio
    #endif // _WIN32_WINNT
    #include <SDKDDKVer.h>
    DISABLE_3RD_PARTY_WARNINGS_START
    #include <Windows.h>
    DISABLE_3RD_PARTY_WARNINGS_END
    #include <VersionHelpers.h>

    #ifdef INCLUDE_SHLOBJ
        #include <ShlObj.h>
        #include <Shellapi.h>
        #pragma comment(lib, "Shell32.lib")
    #endif
#endif
