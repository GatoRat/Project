// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "Xception.h"

/// <summary>
/// A template derivation of Xception
/// </summary>
/// <param name="T_VAL">A simple value less than or equal in size to intptr_t.</param>
template <class T_VAL>
class XceptionT : public Xception
{
public:
    /// <summary>
    /// XceptionT constructor.
    /// </summary>
    /// <param name="code">The code to save with this instance.</param>
    /// <param name="msg">The message to save with this instance.</param>
    /// <param name="getCodeStr">The function to use to get the code string.
    ///     If nullptr the string will be formatted using the default method.</param>
    XceptionT(T_VAL code, const std::string& msg, GetCodeStr getCodeStr = nullptr)
        : Xception(static_cast<intptr_t>(code), msg, getCodeStr)
    {
    }

    /// <summary>
    /// XceptionT constructor.
    /// </summary>
    /// <param name="code">The code to save with this instance.</param>
    /// <param name="pMsg">The message to save with this instance. If nullptr is passed
    ///     an empty string will be saved.</param>
    /// <param name="getCodeStr">The function to use to get the code string.
    ///     If nullptr the string will be formatted using the default method.</param>
    explicit XceptionT(T_VAL code, const char* pMsg = nullptr, GetCodeStr getCodeStr = nullptr)
        : XceptionT(code, std::string(pMsg ? pMsg : ""), getCodeStr)
    {
    }

    /// <summary>
    /// XceptionT destructor.
    /// </summary>
    ~XceptionT() noexcept override = default;

    /// <summary>
    /// Returns the stored code as T_VAL.
    /// </summary>
    /// <returns>The stored code as T_VAL.</returns>
    T_VAL codeAsVal() const noexcept
    {
        return static_cast<T_VAL>(Xception::code());
    }
};
