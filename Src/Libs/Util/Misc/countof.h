// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#ifndef _countof
    #define _countof(array) std::extent<decltype(array)>::value
#endif
