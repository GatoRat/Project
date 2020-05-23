// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <RapidJSON/rapidjson.h>
#include <RapidJSON/error/error.h>
#include "../File/XceptionFileT.h"

#ifndef OUR_RAPIDJSON_MACROS
    #error rapidjson.h must have the following block defined at its top.
        //#ifndef RAPIDJSON_ASSERT
        //    #define RAPIDJSON_ASSERT
        //#endif
        //#ifndef RAPIDJSON_HAS_STDSTRING
        //    #define RAPIDJSON_HAS_STDSTRING 1
        //#endif
        //#define OUR_RAPIDJSON_MACROS
#endif

struct JsonError
{
    enum Code : intptr_t
    {
        Success = 0,
        FileNotFound = 4096, // A high number used to avoid overlap with other codes
        NullOrEmpty,
        FormatError,
        MemberNotFound,
        StringValueEmpty,
        ValueIsNotString,
        ValueIsNotInt,
        ValueIsNotUint,
        ValueIsNotInt64,
        ValueIsNotUint64,
        ValueIsNotFloat,
        ValueIsNotDouble,
        ValueIsNotNumber,
        ValueIsNotBool,
        ValueIsNotArray,
        ValueIsNotObject,
        ValueInvalid,
        ValueOutOfRange,
        ValueIsNull,
        UnknownWriteError,
        UnknownError,
    };

    /// <summary>
    /// Returns the string associated with the passed Code (or rapidjson::ParseErrorCode.)
    /// </summary>
    /// <param name="errorCode">The code for which to get the string.</param>
    /// <param name="returnNullOnError">If the errorCode has no associated string,
    ///     nullptr will be returned instead of the value for UnknownError.</param>
    /// <returns>The string associated with the passed errorCode.</returns>
    static const char* GetString(Code errorCode, bool returnNullOnError = false);

    /// <summary>
    /// Returns the string associated with the passed rapidjson::ParseErrorCode (or Code.)
    /// </summary>
    /// <param name="parseErrorCode">The code for which to get the string.</param>
    /// <param name="returnNullOnError">If the errorCode has no associated string,
    ///     nullptr will be returned instead of the value for UnknownError.</param>
    /// <returns>The string associated with the passed errorCode.</returns>
    static const char* GetString(rapidjson::ParseErrorCode parseErrorCode, bool returnNullOnError = false);

    static std::optional<std::string> GetCodeStr(intptr_t code);
};

class XceptionJson : public XceptionFileT<JsonError::Code>
{
public:
    /// <summary>
    /// XceptionJson constructor.
    /// </summary>
    /// <param name="errorCode">The JsonError::Code to save with this instance.</param>
    /// <param name="pMsg">The message to save with this instance.
    ///     If nullptr is passed in empty string will be saved.</param>
    /// <param name="pStreamName">The stream name where failure occurred.</param>
    explicit XceptionJson(JsonError::Code errorCode, const char* pMsg = nullptr, const char* pStreamName = nullptr);

    /// <summary>
    /// XceptionJson constructor.
    /// </summary>
    /// <param name="parseErrorCode">The rapidjson ParseErrorCode to save.</param>
    /// <param name="pMsg">The message to save with this instance. If nullptr is
    ///     passed an empty string will be saved.</param>
    /// <param name="pStreamName">The stream name where failure occurred.</param>
    explicit XceptionJson(rapidjson::ParseErrorCode parseErrorCode, const char* pMsg = nullptr, const char* pStreamName = nullptr);

    /// <summary>
    /// XceptionJson constructor.
    /// </summary>
    /// <param name="errorCode">The error code to save.</param>
    /// <param name="pMsg">The message to save with this instance. If nullptr is
    ///     passed an empty string will be saved.</param>
    /// <param name="pStreamName">The stream name where failure occurred.</param>
    explicit XceptionJson(intptr_t errorCode, const char* pMsg = nullptr, const char* pStreamName = nullptr);

    /// <summary>
    /// XceptionJson constructor.
    /// </summary>
    /// <param name="path">The file name where failure occurred.</param>
    /// <param name="errorCode">The JsonError::Code to save with this instance.</param>
    /// <param name="pMsg">The message to save with this instance.
    ///     If nullptr is passed in empty string will be saved.</param>
    XceptionJson(const fs::path& path, JsonError::Code errorCode, const char* pMsg = nullptr);

    /// <summary>
    /// XceptionJson constructor.
    /// </summary>
    /// <param name="path">The file name where failure occurred.</param>
    /// <param name="parseErrorCode">The rapidjson ParseErrorCode to save.</param>
    /// <param name="pMsg">The message to save with this instance as a UTF8 string.
    ///     If nullptr is passed in empty string will be saved.</param>
    XceptionJson(const fs::path& path, rapidjson::ParseErrorCode parseErrorCode, const char* pMsg = nullptr);

    /// <summary>
    /// XceptionJson constructor.
    /// </summary>
    /// <param name="path">The file name where failure occurred.</param>
    /// <param name="errorCode">The error code to save with this instance.</param>
    /// <param name="pMsg">The message to save with this instance.
    ///     If nullptr is passed in empty string will be saved.</param>
    XceptionJson(const fs::path& path, intptr_t errorCode, const char* pMsg = nullptr);

    /// <summary>
    /// Returns the stream name.
    /// </summary>
    /// <returns>The stream name (may contain nullptr).</returns>
    std::shared_ptr<fs::path> getStreamName() const noexcept;
};
