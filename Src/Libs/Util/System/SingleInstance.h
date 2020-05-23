// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <string_view>

class SingleInstance final
{
public:
    SingleInstance() = default;
    SingleInstance(const SingleInstance&) = delete;
    void operator=(const SingleInstance&) = delete;
    ~SingleInstance();

    /**
        @param windowName Name of application. Must be less than 246 characters.
    */
    bool lock(std::string_view baseWindowTitle);

private:
    void* _hMutex = nullptr;
};
