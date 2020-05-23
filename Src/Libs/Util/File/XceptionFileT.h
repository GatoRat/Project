// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "XceptionFile.h"

/** A template derivation of XceptionFile
    This derives from XceptionFile, which is derived from Xception,
    NOT ios::failure.
 */
template <class T_VAL>
class XceptionFileT : public XceptionFile
{
public:
    /** XceptionFileT constructor.
       @param path The path to save with this instance.
       @param error The code to save with this instance.
       @param pMsg The message to save with this instance. If nullptr is passed, an
            empty string will be saved.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
     */
    explicit XceptionFileT(const fs::path& path, T_VAL code = 0, const char* pMsg = nullptr, GetCodeStr getCodeStr = GetErrStrErrno)
        : XceptionFile(path, static_cast<intptr_t>(code), pMsg, getCodeStr)
    {
    }

    /** XceptionFileT constructor.
       @param path The path to save with this instance.
       @param error The code to save with this instance.
       @param msg The message to save with this instance.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
       */
    XceptionFileT(const fs::path& path, T_VAL code, const std::string& msg, GetCodeStr getCodeStr = GetErrStrErrno) noexcept
        : XceptionFileT(path, static_cast<intptr_t>(code), msg.c_str(), getCodeStr)
    {
    }

    /** XceptionFileT constructor.
       @param pPath The path to save with this instance. May be nullptr.
       @param error The code to save with this instance.
       @param pMsg The message to save with this instance. If nullptr is passed, an
            empty string will be saved.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
     */
    explicit XceptionFileT(const char* pPath, T_VAL code = 0, const char* pMsg = nullptr, GetCodeStr getCodeStr = GetErrStrErrno)
        : XceptionFile(pPath, static_cast<intptr_t>(code), pMsg, getCodeStr)
    {
    }

    /** XceptionFileT constructor.
       @param pPath The path to save with this instance. May be nullptr.
       @param error The code to save with this instance.
       @param pMsg The message to save with this instance. If nullptr is passed, an
            empty string will be saved.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
     */
    explicit XceptionFileT(const wchar_t* pPath, T_VAL code = 0, const char* pMsg = nullptr, GetCodeStr getCodeStr = GetErrStrErrno) noexcept
        : XceptionFile(pPath, static_cast<intptr_t>(code), pMsg, getCodeStr)
    {
    }

    /** XceptionFileT constructor.
       @param error The code to save with this instance.
       @param pMsg The message to save with this instance. If nullptr is passed, an
            empty string will be saved.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
       @remarks The shared pointer returnd by \a path will contain a nullptr.
     */
    explicit XceptionFileT(T_VAL code, const char* pMsg = nullptr, GetCodeStr getCodeStr = GetErrStrErrno) noexcept
        : XceptionFile(static_cast<intptr_t>(code), pMsg, getCodeStr)
    {
    }

    /** XceptionFile constructor.
       @param code The code to save with this instance.
       @param msg The message to save with this instance.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
       @remarks The shared pointer returnd by \a path will contain a nullptr.
     */
    XceptionFileT(T_VAL code, const std::string& msg, GetCodeStr getCodeStr = GetErrStrErrno) noexcept
        : XceptionFile(static_cast<intptr_t>(code), msg, getCodeStr)
    {
    }

    /** XceptionFileT destructor.
     */
    ~XceptionFileT() noexcept override = default;

    /// <summary>
    /// Returns the stored code as T_VAL.
    /// </summary>
    /// <returns>The stored code as T_VAL.</returns>
    T_VAL codeAsVal() const noexcept
    {
        return static_cast<T_VAL>(XceptionFile::code());
    }
};
