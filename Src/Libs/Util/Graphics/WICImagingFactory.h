// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

// The following must be done if in header to prevent tons of warnings:
// ...\shared\rpcndr.h(192): error C2872: 'byte': ambiguous symbol
//#define byte win_byte_override
#include <atlbase.h>
#include <wincodec.h>
//#undef byte

namespace WICImaging
{
    IWICImagingFactory* GetFactory();
}
