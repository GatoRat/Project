// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include <RapidJSON/rapidjson.h>
#include <RapidJSON/document.h>
#include "JsonStringHandler.h"
#include "XceptionJson.h"
#include "../String/Convert.h"
#include "../String/Str.h"

namespace fs = std::filesystem;

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

/// <summary>
/// Returns the Value::ConstMemberIterator for the matching value.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>If the member is found, the Value::ConstMemberIterator for that value.</returns>
/// <exception cref="XceptionJson">Thrown if key cannot be found.</exception>
template <typename Encoding, typename Allocator>
rapidjson::Value::ConstMemberIterator JsonFindMemberEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                                       const char* pStreamName = nullptr)
{
    rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
    if (memberIt == value.MemberEnd())
    {
        throw XceptionJson(JsonError::MemberNotFound, pName, pStreamName);
    }
    return memberIt;
}

/// <summary>
/// Returns a pointer to the string value associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="failIfEmpty">Throws exception if the string is empty.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>A char pointer if the member is found and it is type string.</returns>
/// <exception cref="XceptionJson">Thrown if key cannot be found, value isn't a
///     string or, if failIfEmpty is set, if the value is empty.</exception>
template <typename Encoding, typename Allocator>
const char* JsonGetStringEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, bool failIfEmpty = false,
                            const char* pStreamName = nullptr)
{
    const char* pStr = nullptr;

    const auto memberIt = JsonFindMemberEx(value, pName);
    if (!memberIt->value.IsNull())
    {
        if (!memberIt->value.IsString())
        {
            throw XceptionJson(JsonError::ValueIsNotString, pName, pStreamName);
        }

        pStr = memberIt->value.GetString();
    }

    if (failIfEmpty && Str::IsNullOrEmpty(pStr))
    {
        throw XceptionJson(JsonError::StringValueEmpty, pName, pStreamName);
    }

    return pStr;
}

/// <summary>
/// Returns the string value associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="failIfEmpty">Throws exception if the string is empty.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>A char pointer if the member is found and it is type string.</returns>
/// <exception cref="XceptionJson">Thrown if key cannot be found, value isn't a
///     string or, if failIfEmpty is set, if the value is empty.</exception>
template <typename Encoding, typename Allocator>
std::string JsonGetStdStringEx(const rapidjson::GenericValue<Encoding, Allocator>& value,
                               const char* pName, bool failIfEmpty = false, const char* pStreamName = nullptr)
{
    return Convert::ToStr(JsonGetStringEx(value, pName, failIfEmpty, pStreamName));
}

/// <summary>
/// Returns the string value associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="handler">An optional function to modify the string view. Typically used
///     to trim the string.</param>
/// <param name="failIfEmpty">Throws exception if the string is empty.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>A char pointer if the member is found and it is type string.</returns>
/// <exception cref="XceptionJson">Thrown if key cannot be found, value isn't a
///     string or, if failIfEmpty is set, if the value is empty.</exception>
template <typename Encoding, typename Allocator>
std::string_view JsonGetStringViewEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                     JsonStringViewHandler handler, bool failIfEmpty = false, const char* pStreamName = nullptr)
{
    std::string_view str;
    auto pStr = JsonGetStringEx(value, pName, failIfEmpty, pStreamName);
    if (pStr)
    {
        str = pStr;

        if (handler)
        {
            str = handler(str);
            if (failIfEmpty && str.empty())
            {
                throw XceptionJson(JsonError::StringValueEmpty, pName, pStreamName);
            }
        }
    }
    return str;
}

/// <summary>
/// Returns the trimmed string value associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="failIfEmpty">Throws exception if the string is empty.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>A char pointer if the member is found and it is type string.</returns>
/// <exception cref="XceptionJson">Thrown if key cannot be found, value isn't a
///     string or, if failIfEmpty is set, if the value is empty.</exception>
template <typename Encoding, typename Allocator>
std::string_view JsonGetTrimmedStringEx(const rapidjson::GenericValue<Encoding, Allocator>& value,
                                        const char* pName, bool failIfEmpty = false, const char* pStreamName = nullptr)
{
    return JsonGetStringViewEx(value, pName, [](std::string_view str)->std::string_view
    {
        return Str::TrimView(str);

    }, failIfEmpty, pStreamName);
}

/// <summary>
/// Returns an int value associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">The optional stream name, for logging and debugging purposes.</param>
/// <returns>The value if the member is found and it is type int.</returns>
/// <exception cref="XceptionJson">Thrown if key cannot be found or isn't an
///     int.</exception>
template <typename Encoding, typename Allocator>
int JsonGetIntEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, const char* pStreamName = nullptr)
{
    const auto memberIt = JsonFindMemberEx(value, pName, pStreamName);
    if (!memberIt->value.IsInt())
    {
        throw XceptionJson(JsonError::ValueIsNotInt, pName, pStreamName);
    }
    return memberIt->value.GetInt();
}

/// <summary>
/// Returns an unsigned int value associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>The value if the member is found and it is type unsigned.</returns>
/// <exception cref="XceptionJson">Thrown if key cannot be found or isn't an
///     unsigned int.</exception>
template <typename Encoding, typename Allocator>
unsigned JsonGetUintEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, const char* pStreamName = nullptr)
{
    const rapidjson::Value::ConstMemberIterator memberIt = JsonFindMemberEx(value, pName, pStreamName);
    if (!memberIt->value.IsUint())
    {
        throw XceptionJson(JsonError::ValueIsNotUint, pName, pStreamName);
    }
    return memberIt->value.GetUint();
}

/// <summary>
/// Returns an int64 value associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>The value if the member is found and it is type int64.</returns>
/// <exception cref="XceptionJson">Thrown if key cannot be found or isn't an int64.</exception>
template <typename Encoding, typename Allocator>
int64_t JsonGetInt64Ex(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, const char* pStreamName = nullptr)
{
    const rapidjson::Value::ConstMemberIterator memberIt = JsonFindMemberEx(value, pName, pStreamName);
    if (!memberIt->value.IsInt64())
    {
        throw XceptionJson(JsonError::ValueIsNotInt64, pName, pStreamName);
    }
    return memberIt->value.GetInt64();
}

/// <summary>
/// Returns the unsigned int64 value associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>The value if the member is found and it is type uint64.</returns>
/// <exception cref="XceptionJson">Thrown if key cannot be found or isn't a uint64.</exception>
template <typename Encoding, typename Allocator>
uint64_t JsonGetUint64Ex(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, const char* pStreamName = nullptr)
{
    const rapidjson::Value::ConstMemberIterator memberIt = JsonFindMemberEx(value, pName, pStreamName);
    if (!memberIt->value.IsUint64())
    {
        throw XceptionJson(JsonError::ValueIsNotUint64, pName, pStreamName);
    }
    return memberIt->value.GetUint64();
}

/// <summary>
/// Returns the double value associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="strict">If <b>true</b>, the value <b>must</b> contain a decimal
///     point, otherwise any number will be valid.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>The value if the member is found and it is type double.</returns>
/// <exception cref="XceptionJson">Thrown if key cannot be found or isn't a double.</exception>
template <typename Encoding, typename Allocator>
double JsonGetDoubleEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, bool strict = false,
                       const char* pStreamName = nullptr)
{
    rapidjson::Value::ConstMemberIterator memberIt = JsonFindMemberEx(value, pName, pStreamName);
    if (strict)
    {
        if (!memberIt->value.IsDouble())
        {
            throw XceptionJson(JsonError::ValueIsNotDouble, pName, pStreamName);
        }
    }
    else
    {
        if (!memberIt->value.IsNumber())
        {
            throw XceptionJson(JsonError::ValueIsNotNumber, pName, pStreamName);
        }
    }
    return memberIt->value.GetDouble();
}

/// <summary>
/// Returns the float value associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="strict">If <b>true</b>, the value <b>must</b> contain a decimal
///     point, otherwise any number will be valid.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>The value if the member is found and it is type double.</returns>
/// <exception cref="XceptionJson">Thrown if key cannot be found or isn't a double.</exception>
template <typename Encoding, typename Allocator>
float JsonGetFloatEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, bool strict = false,
                     const char* pStreamName = nullptr)
{
    const auto memberIt = JsonFindMemberEx(value, pName, pStreamName);
    if (strict)
    {
        if (!memberIt->value.IsFloat())
        {
            throw XceptionJson(JsonError::ValueIsNotDouble, pName, pStreamName);
        }
    }
    else
    {
        if (!memberIt->value.IsNumber())
        {
            throw XceptionJson(JsonError::ValueIsNotNumber, pName, pStreamName);
        }
    }
    return memberIt->value.GetFloat();
}

/// <summary>
/// Returns the bool value associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>The value if the member is found and it is type bool.</returns>
/// <exception cref="XceptionJson">Thrown if key cannot be found or isn't a bool.</exception>
template <typename Encoding, typename Allocator>
bool JsonGetBoolEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, const char* pStreamName = nullptr)
{
    const auto memberIt = JsonFindMemberEx(value, pName, pStreamName);
    if (!memberIt->value.IsBool())
    {
        throw XceptionJson(JsonError::ValueIsNotBool, pName, pStreamName);
    }
    return memberIt->value.GetBool();
}

/// <summary>
/// Returns the array object associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>The value if the member is found and it is type array.</returns>
/// <exception cref="XceptionJson">Thrown if key cannot be found or isn't an array.</exception>
template <typename Encoding, typename Allocator>
const rapidjson::Value& JsonGetArrayEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                       const char* pStreamName = nullptr)
{
    const rapidjson::Value::ConstMemberIterator memberIt = JsonFindMemberEx(value, pName, pStreamName);
    if (!memberIt->value.IsArray())
    {
        throw XceptionJson(JsonError::ValueIsNotArray, pName, pStreamName);
    }
    return memberIt->value;
}

/// <summary>
/// Returns the object associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>The value if the member is found and it is type object.</returns>
/// <exception cref="XceptionJson">Thrown if key cannot be found or isn't an object.</exception>
template <typename Encoding, typename Allocator>
const rapidjson::Value& JsonGetObjectEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                        const char* pStreamName = nullptr)
{
    const rapidjson::Value::ConstMemberIterator memberIt = JsonFindMemberEx(value, pName, pStreamName);
    if (!memberIt->value.IsObject())
    {
        throw XceptionJson(JsonError::ValueIsNotObject, pName, pStreamName);
    }

    return memberIt->value;
}

/// <summary>
/// Indicates whether the value associated with pName is a null.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns><b>true</b> if the member is found and it is a null. <b>false</b>.</returns>
/// <exception cref="XceptionJson">Thrown if key cannot be found .</exception>
template <typename Encoding, typename Allocator>
bool JsonIsNullEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, const char* pStreamName = nullptr)
{
    const rapidjson::Value::ConstMemberIterator memberIt = JsonFindMemberEx(value, pName, pStreamName);
    return memberIt->value.IsNull();
}

/// <summary>
/// Indicates whether the value associated with pName in an array.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns><b>true</b> if the member is found and it is an array.</returns>
/// <exception cref="XceptionJson">Thrown if key cannot be found.</exception>
template <typename Encoding, typename Allocator>
bool JsonIsArrayEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, const char* pStreamName = nullptr)
{
    const auto memberIt = JsonFindMemberEx(value, pName, pStreamName);
    return memberIt->value.IsArray();
}

/// <summary>
/// Indicates whether the value associated with pName in a number.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns><b>true</b> if the member is found and it is a number.</returns>
/// <exception cref="XceptionJson">Thrown if key cannot be found.</exception>
template <typename Encoding, typename Allocator>
bool JsonIsNumberEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, const char* pStreamName = nullptr)
{
    const auto memberIt = JsonFindMemberEx(value, pName, pStreamName);
    return memberIt->value.IsNumber();
}

/// <summary>
/// Returns the Value::ConstMemberIterator for the matching value.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName1">Primary name of the value.</param>
/// <param name="pName2">Alternate name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <param name="pFoundFirst">If not null, set to <b>true</b> if \p pName1 found, <b>false</b> if \p pName2 found.</param>
/// <returns>If the member is found, the Value::ConstMemberIterator for that value.</returns>
/// <exception cref="XceptionJson">Thrown if \p pName1 or \p pName2 cannot be found.</exception>
template <typename Encoding, typename Allocator>
rapidjson::Value::ConstMemberIterator JsonFindMemberEx_two(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName1,
                                                           const char* pName2, const char* pStreamName = nullptr, bool* pFoundFirst = nullptr)
{
    auto memberIt = value.FindMember(pName1);
    if (memberIt == value.MemberEnd())
    {
        memberIt = value.FindMember(pName2);
        if (memberIt == value.MemberEnd())
        {
            std::string name(pName1);
            name += " or ";
            name += pName2;

            throw XceptionJson(JsonError::MemberNotFound, name.c_str(), pStreamName);
        }
        else if (pFoundFirst)
        {
            *pFoundFirst = false;
        }
    }
    else if (pFoundFirst)
    {
        *pFoundFirst = true;
    }
    return memberIt;
}

template <typename Encoding, typename Allocator>
const rapidjson::Value& JsonFindNumberEx_two(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName1,
                                             const char* pName2, const char* pStreamName = nullptr, bool* pFoundFirst = nullptr)
{
    const auto memberIt = JsonFindMemberEx_two(value, pName1, pName2, pStreamName, pFoundFirst);

    if (!memberIt->value.IsNumber())
    {
        std::string name(pName1);
        name += " or ";
        name += pName2;

        throw XceptionJson(JsonError::ValueIsNotNumber, name.c_str(), pStreamName);
    }

    return memberIt->value;
}

template <typename Encoding, typename Allocator>
int32_t JsonGetIntEx_two(const rapidjson::GenericValue<Encoding, Allocator>& value,
                         const char* pName1, const char* pName2, const char* pStreamName = nullptr, bool* pFoundFirst = nullptr)
{
    return JsonFindNumberEx_two(value, pName1, pName2, pStreamName, pFoundFirst).GetInt();
}

template <typename Encoding, typename Allocator>
uint32_t JsonGetUintEx_two(const rapidjson::GenericValue<Encoding, Allocator>& value,
                           const char* pName1, const char* pName2, const char* pStreamName = nullptr, bool* pFoundFirst = nullptr)
{
    return JsonFindNumberEx_two(value, pName1, pName2, pStreamName, pFoundFirst).GetUint();
}

template <typename Encoding, typename Allocator>
float JsonGetFloatEx_two(const rapidjson::GenericValue<Encoding, Allocator>& value,
                         const char* pName1, const char* pName2, const char* pStreamName = nullptr, bool* pFoundFirst = nullptr)
{
    return JsonFindNumberEx_two(value, pName1, pName2, pStreamName, pFoundFirst).GetFloat();
}

namespace JsonNonConst
{
    /// <summary>
    /// Returns the Value::MemberIterator for the matching value.
    /// </summary>
    /// <param name="value">A rapidjson::GenericValue.</param>
    /// <param name="pName">The name of the value.</param>
    /// <returns>If the member is found, the Value::MemberIterator for that value.</returns>
    /// <exception cref="XceptionJson">Thrown if key cannot be found.</exception>
    template <typename Encoding, typename Allocator>
    rapidjson::Value::MemberIterator FindMemberEx(rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
    {
        auto memberIt = value.FindMember(pName);
        if (memberIt == value.MemberEnd())
        {
            throw XceptionJson(JsonError::MemberNotFound, pName);
        }
        return memberIt;
    }

    /// <summary>
    /// Returns the array object associated with pName.
    /// </summary>
    /// <param name="value">A rapidjson::GenericValue.</param>
    /// <param name="pName">The name of the value.</param>
    /// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
    /// <returns>The value if the member is found and it is type array.</returns>
    /// <exception cref="XceptionJson">Thrown if key cannot be found or isn't an array.</exception>
    template <typename Encoding, typename Allocator>
    rapidjson::Value& GetArrayEx(rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
    {
        auto memberIt = FindMemberEx(value, pName);
        if (!memberIt->value.IsArray())
        {
            throw XceptionJson(JsonError::ValueIsNotArray, pName);
        }
        return memberIt->value;
    }
}
