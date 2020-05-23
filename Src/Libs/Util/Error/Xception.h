// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include <functional>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>
#include "ErrString.h"

namespace fs = std::filesystem;

/** An exception class unique to this application.
    This derives from std::runtime_error.
 */
class Xception : public std::runtime_error
{
public:
    typedef std::function<std::optional<std::string>(intptr_t code)> GetCodeStr;

public:
    /** Xception constructor.
       @param code The code to save with this instance.
       @param pMsg The message to save with this instance. If nullptr is passed, an
            empty string will be saved.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
     */
    explicit Xception(intptr_t code, const char* pMsg = nullptr, GetCodeStr getCodeStr = GetErrStr)
        : runtime_error(pMsg ? pMsg : "")
        , _code(code)
        , _getCodeStr(std::move(getCodeStr))
    {
    }

    /** Xception constructor.
       @param code The code to save with this instance.
       @param msg The message to save with this instance.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
     */
    Xception(intptr_t code, const std::string& msg, GetCodeStr getCodeStr = GetErrStr)
        : Xception(code, msg.c_str(), std::move(getCodeStr))
    {
    }

    /** Xception constructor.
       @param pMsg The message to save with this instance. If nullptr is passed, an
            empty string will be saved.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
     */
    explicit Xception(const char* pMsg, GetCodeStr getCodeStr = GetErrStr)
        : Xception(0, pMsg, std::move(getCodeStr))
    {
    }

    /** Xception constructor.
       @param msg The message to save with this instance.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
     */
    explicit Xception(const std::string& msg, GetCodeStr getCodeStr = GetErrStr)
        : Xception(0, msg, std::move(getCodeStr))
    {
    }

    /** Xception constructor.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
    */
    explicit Xception(GetCodeStr getCodeStr = GetErrStr)
        : Xception(0, nullptr, std::move(getCodeStr))
    {
    }

    /** Returns the stored code.
       @returns The stored code.
     */
    virtual intptr_t code() const noexcept
    {
        return _code;
    }

    /** Returns the string associated with the stored code.
       @returns The string associated with the stored code.
     */
    virtual std::string codeStr() const;

    /** Returns the string associated with the stored code.
       @returns The string associated with the stored code.
     */
    virtual std::optional<std::string> codeStrOpt() const;

    /** Indicates whether there is a non-empty what() string.
       @returns  \b true if there is a non-empty what() string, else \b false.
     */
    bool hasWhat() const;

    /** Returns this error as a string.
       @param includePathInfo If \b true and this will ultimately call XceptionFile,
            path information will be included.
       @returns This error as a string.
     */
    virtual std::string errStr(const char* pSuffix = nullptr, bool includePathInfo = true) const;

    /** Writes this error to a stream.
       @param stream A reference to an ostream object.
       @returns The passed ostream object reference.
     */
    virtual std::ostream& write(std::ostream& stream) const;

    /** Sets or replaces the stored path.
        @param path The path to save with this instance.
        @remarks Only has an effect if this is a base instance of XceptionFile.
    */
    virtual void setPath(const fs::path& path);

    virtual const fs::path* getPath() const;

public:
    intptr_t _code = 0;
    GetCodeStr _getCodeStr;
};

inline
std::ostream& operator<<(std::ostream& stream, const Xception& xception)
{
    return xception.write(stream);
}
