// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <functional>

/// <summary>A class to mimic a Finally clause.</summary>
/// <remarks> Example code:
/// HANDLE hFile = INVALID_HANDLE_VALUE;
/// Finally finally([&]()
/// {
///     if (hFile != INVALID_HANDLE_VALUE)
///     {
///         CloseHandle(hFile);
///     }
/// });
///
/// This is mean to be very simple and lightweight, thus this class is marked as final
/// and is not virtual.
/// </remarks>
class Finally final
{
public:
    typedef std::function<void()> Finalizer;

public:
    /// <summary>Finally constructor.</summary>
    /// <param name="finalizer">The finalizer function.</param>
    explicit Finally(Finalizer finalizer)
        : _finalizer(move(finalizer))
    {
    }
    Finally(const Finally& other) = delete;
    void operator=(const Finally&) = delete;

    /// <summary>Finally destructor.</summary>
    /// <remarks>
    /// If the finalizer is not null, it will be called.
    /// </remarks>
    ~Finally()
    {
        if (_finalizer)
        {
            _finalizer();
        }
    }

    /// <summary>
    /// Prevent the finalizer function from being called.
    /// </summary>
    void dismiss() noexcept
    {
        _finalizer = nullptr;
    }

private:
    Finalizer _finalizer;
};
