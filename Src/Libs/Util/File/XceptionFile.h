// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include "../Error/Xception.h"
#include "../String/Convert.h"

namespace fs = std::filesystem;

/** A file exception class unique to this application.
    This derives from Xception, NOT ios::failure.
 */
class XceptionFile : public Xception
{
public:
    /** XceptionFile constructor.
       @param path The path to save with this instance.
       @param code The code to save with this instance.
       @param pMsg The message to save with this instance. If nullptr is passed, an
            empty string will be saved.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
     */
    XceptionFile(const fs::path& path, intptr_t code, const char* pMsg = nullptr, GetCodeStr getCodeStr = GetErrStrErrno)
        : Xception(code, pMsg, std::move(getCodeStr))
        , _path(std::make_shared<fs::path>(path))
    {
    }

    /** XceptionFile constructor.
       @param path The path to save with this instance.
       @param code The code to save with this instance.
       @param msg The message to save with this instance.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
       */
    XceptionFile(const fs::path& path, intptr_t code, const std::string& msg, GetCodeStr getCodeStr = GetErrStrErrno)
        : XceptionFile(path, code, msg.c_str(), std::move(getCodeStr))
    {
    }

    /** XceptionFile constructor.
       @param path The path to save with this instance.
       @param pMsg The message to save with this instance. If nullptr is passed, an
            empty string will be saved.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
     */
    explicit XceptionFile(const fs::path& path, const char* pMsg = nullptr, GetCodeStr getCodeStr = GetErrStrErrno)
        : Xception(static_cast<intptr_t>(errno), pMsg, std::move(getCodeStr))
        , _path(std::make_shared<fs::path>(path))
    {
    }

    /** XceptionFile constructor.
       @param path The path to save with this instance.
       @param msg The message to save with this instance.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
       */
    XceptionFile(const fs::path& path, const std::string& msg, GetCodeStr getCodeStr = GetErrStrErrno)
        : XceptionFile(path, msg.c_str(), std::move(getCodeStr))
    {
    }

    /** XceptionFile constructor.
       @param pPath The path to save with this instance. May be nullptr.
       @param code The code to save with this instance.
       @param pMsg The message to save with this instance. If nullptr is passed, an
            empty string will be saved.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
     */
    explicit XceptionFile(const char* pPath, intptr_t code = 0, const char* pMsg = nullptr, GetCodeStr getCodeStr = GetErrStrErrno)
        : Xception(code, pMsg, std::move(getCodeStr))
    {
        if (pPath)
        {
            _path = std::make_shared<fs::path>(pPath);
        }
    }

    /** XceptionFile constructor.
       @param pPath The path to save with this instance. May be nullptr.
       @param code The code to save with this instance.
       @param pMsg The message to save with this instance. If nullptr is passed, an
            empty string will be saved.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
     */
    explicit XceptionFile(const wchar_t* pPath, intptr_t code = 0, const char* pMsg = nullptr, GetCodeStr getCodeStr = GetErrStrErrno)
        : Xception(code, pMsg, std::move(getCodeStr))
    {
        if (pPath)
        {
            _path = std::make_shared<fs::path>(pPath);
        }
    }

    /** XceptionFile constructor.
       @param code The code to save with this instance.
       @param pMsg The message to save with this instance. If nullptr is passed, an
            empty string will be saved.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
       @remarks The shared pointer returnd by \a path will contain a nullptr.
     */
    explicit XceptionFile(intptr_t code, const char* pMsg = nullptr, GetCodeStr getCodeStr = GetErrStrErrno)
        : Xception(code, pMsg, std::move(getCodeStr))
    {
    }

    /** XceptionFile constructor.
       @param code The code to save with this instance.
       @param msg The message to save with this instance.
       @param getCodeStr The function to use to get the code string. If nullptr the
            string will be formatted using the default method.
       @remarks The shared pointer returnd by \a path will contain a nullptr.
     */
    XceptionFile(intptr_t code, const std::string& msg, GetCodeStr getCodeStr = GetErrStrErrno)
        : XceptionFile(code, msg.c_str(), std::move(getCodeStr))
    {
    }

    /** XceptionFile destructor.
     */
    ~XceptionFile() noexcept override = default;

    /** Sets or replaces the stored path.
       @param path The path to save with this instance.
    */
    void setPath(const fs::path& path) override
    {
        _path = std::make_shared<fs::path>(path);
    }

    /** Returns the stored path.
       @returns The stored path (may contain nullptr).
     */
    std::shared_ptr<fs::path> path() const noexcept
    {
        return _path;
    }

    /** Indicates whether there is a non-empty stored path.
       @returns \b true if the stored path is not nullptr, else \b false.
     */
    bool hasPath() const noexcept
    {
        return static_cast<bool>(_path) && !_path->empty();
    }

    /** DoCallback this error as a string.
        @param pSuffix If not null or empty, appends ": " and this string.
        @param includePathInfo If \b true, path information will be included.
        @returns This error as a string.
     */
    std::string errStr(const char* pSuffix = nullptr, bool includePathInfo = true) const override
    {
        std::string str(Xception::errStr());

        if (includePathInfo)
        {
            if (!_path)
            {
                str += ": <path is null>";
            }
            else if (_path->empty())
            {
                str += ": <path is empty>";
            }
            else
            {
                str += ": \"";
                str += _path->u8string(); //Convert::ToStr(_path->u8string());
                str += '"';
            }
        }

        if (pSuffix && *pSuffix)
        {
            if (!str.empty())
            {
                str += ": ";
            }
            str += pSuffix;
        }

        return str;
    }

    const fs::path* getPath() const override
    {
        return _path.get();
    }

private:
    // TODO: Should this be changed to std::optional?
    std::shared_ptr<fs::path> _path;
};
