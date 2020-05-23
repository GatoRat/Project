//
// asio.cpp
// ~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#define ASIO_STANDALONE             1
#define ASIO_SEPARATE_COMPILATION   1
#define _WIN32_WINNT                0x0601
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING

#pragma warning( push )
#pragma warning( disable : 4548 )

#include "asio/impl/src.hpp"

#pragma warning( pop )