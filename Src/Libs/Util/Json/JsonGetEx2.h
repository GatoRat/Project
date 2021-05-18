// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <RapidJSON/rapidjson.h>
#include <RapidJSON/document.h>
#include "JsonStringHandler.h"
#include "XceptionJson.h"

/// <summary>
/// Returns a const char* associated with pName or pDefault if pName doesn't exist.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pDefault">The string to use if pName doesn't exist.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>A std::optional&lt;const char*&gt; instance.</returns>
/// <exception cref="JsonException">Thrown if the value exists and isn't a string.</exception>
template <typename Encoding, typename Allocator>
const char* JsonGetStringEx2(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, const char* pDefault = nullptr,
                             const char* pStreamName = nullptr)
{
    const rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
    if (memberIt == value.MemberEnd())
    {
        // Not finding the member is okay.
    }
    else if (!memberIt->value.IsString())
    {
        throw XceptionJson(JsonError::ValueIsNotString, pName, pStreamName);
    }
    else
    {
        pDefault = memberIt->value.GetString();
    }

    return pDefault;
};

/// <summary>
/// Returns a const char* associated with pName or pDefault if pName doesn't exist or is null or empty.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pDefault">The string to use if pName doesn't exist or is null or empty.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>A std::optional&lt;const char*&gt; instance.</returns>
/// <exception cref="JsonException">Thrown if the value exists and isn't a string or null.</exception>
template <typename Encoding, typename Allocator>
const char* JsonGetStringEx3(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                             const char* pDefault = nullptr, const char* pStreamName = nullptr)
{
    const rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
    if (memberIt == value.MemberEnd() || memberIt->value.IsNull())
    {
        // Not finding the member is okay as is if member is null.
    }
    else if (!memberIt->value.IsString()) //-V807
    {
        throw XceptionJson(JsonError::ValueIsNotString, pName, pStreamName);
    }
    else if (memberIt->value.GetStringLength())
    {
        pDefault = memberIt->value.GetString();
    }

    return pDefault;
};

/// <summary>
/// Returns the std::string associated with pName or pDefault if pName doesn't exist.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pDefault">The string to use if pName doesn't exist. If nullptr and
///     pName doesn't exist, an empty string will be used.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>The std::string associated with pName or pDefault if pName doesn't exist.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't a string.</exception>
template <typename Encoding, typename Allocator>
std::string JsonGetStdStringEx2(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                const char* pDefault = nullptr, const char* pStreamName = nullptr)
{
    pDefault = JsonGetStringEx2(value, pName, pDefault, pStreamName);
    return std::string(pDefault ? pDefault : "");
};

/// <summary>
/// Returns the std::string associated with pName or default if pName doesn't exist or is null or empty.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="default">The string to use if pName doesn't exist or is null or empty.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>The std::string associated with pName or default if pName doesn't exist or is empty.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't a string.</exception>
template <typename Encoding, typename Allocator>
std::string JsonGetStdStringEx3(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                std::string& defaultVal, const char* pStreamName = nullptr)
{
    auto pStr = JsonGetStringEx3(value, pName, nullptr, pStreamName);
    return pStr && *pStr ? std::string(pStr) : defaultVal;
};

/// <summary>
/// Returns the std::string_view associated with pName or pDefault if pName doesn't exist.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="handler">An optional function to modify the string view. Typically used
///     to trim the string.</param>
/// <param name="pDefault">The string to use if pName doesn't exist. If nullptr and
///     pName doesn't exist, an empty string will be used.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>The std::string associated with pName or pDefault if pName doesn't exist.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't a string.</exception>
template <typename Encoding, typename Allocator>
std::string_view JsonGetStringViewEx2(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                      JsonStringViewHandler handler, const char* pDefault = "", const char* pStreamName = nullptr)
{
    pDefault = JsonGetStringEx2(value, pName, pDefault, pStreamName);
    std::string_view str{ pDefault ? pDefault : "" };
    if (handler)
    {
        str = handler(str);
    }
    return str;
}

/// <summary>
/// Returns the std::string_view associated with pName or pDefault if pName doesn't exist.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pDefault">The string to use if pName doesn't exist. If nullptr and
///     pName doesn't exist, an empty string will be used.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>The std::string associated with pName or pDefault if pName doesn't exist.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't a string.</exception>
template <typename Encoding, typename Allocator>
std::string_view JsonGetTrimmedStringEx2(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                         const char* pDefault = "", const char* pStreamName = nullptr)
{
    return JsonGetStringViewEx2(value, pName, [](std::string_view str)->std::string_view
    {
        return Str::TrimView(str);

    }, pDefault, pStreamName);
}

/// <summary>
/// Returns the int associated with pName, default if pName doesn't exist.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="defaultVal">The value to use if pName doesn't exist.</param>
/// <returns>The int associated with pName, default if pName doesn't exist.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't an int.</exception>
template <typename Encoding, typename Allocator>
int JsonGetIntEx2(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, int defaultVal = 0,
                  const char* pStreamName = nullptr)
{
    const rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
    if (memberIt == value.MemberEnd())
    {
        // Not finding the member is okay.
    }
    else if (!memberIt->value.IsInt())
    {
        throw XceptionJson(JsonError::ValueIsNotInt, pName, pStreamName);
    }
    else
    {
        defaultVal = memberIt->value.GetInt();
    }

    return defaultVal;
};

/// <summary>
/// Returns the unsigned int associated with pName, default if pName doesn't exist.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>The unsigned int associated with pName, default if pName doesn't exist.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't an unsigned int.</exception>
template <typename Encoding, typename Allocator>
unsigned JsonGetUintEx2(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, unsigned defaultVal = 0,
                        const char* pStreamName = nullptr)
{
    const rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
    if (memberIt == value.MemberEnd())
    {
        // Not finding the member is okay.
    }
    else if (!memberIt->value.IsUint())
    {
        throw XceptionJson(JsonError::ValueIsNotUint, pName, pStreamName);
    }
    else
    {
        defaultVal = memberIt->value.GetUint();
    }

    return defaultVal;
};

/// <summary>
/// Returns the int64_t associated with pName, default if pName doesn't exist.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="defaultVal">The value to use if pName doesn't exist.</param>
/// <returns>The int64_t associated with pName, default if pName doesn't exist.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't an int64_t.</exception>
template <typename Encoding, typename Allocator>
int64_t JsonGetInt64Ex2(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, int64_t defaultVal = 0,
                        const char* pStreamName = nullptr)
{
    const auto memberIt = value.FindMember(pName);
    if (memberIt == value.MemberEnd())
    {
        // Not finding the member is okay.
    }
    else if (!memberIt->value.IsInt64())
    {
        throw XceptionJson(JsonError::ValueIsNotInt64, pName, pStreamName);
    }
    else
    {
        defaultVal = memberIt->value.GetInt64();
    }

    return defaultVal;
};

/// <summary>
/// Returns the uint64_t associated with pName, default if pName doesn't exist.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="defaultVal">The value to use if pName doesn't exist.</param>
/// <returns>The uint64_t associated with pName, default if pName doesn't exist.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't an uint64_t.</exception>
template <typename Encoding, typename Allocator>
uint64_t JsonGetUint64Ex2(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, uint64_t defaultVal = 0,
                          const char* pStreamName = nullptr)
{
    rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
    if (memberIt == value.MemberEnd())
    {
        // Not finding the member is okay.
    }
    else if (!memberIt->value.IsUint64())
    {
        throw XceptionJson(JsonError::ValueIsNotUint64, pName, pStreamName);
    }
    else
    {
        defaultVal = memberIt->value.GetUint64();
    }

    return defaultVal;
};

/// <summary>
/// Returns the double associated with pName, default if pName doesn't exist.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="defaultVal">The value to use if pName doesn't exist.</param>
/// <returns>The double associated with pName, default if pName doesn't exist.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't an double.</exception>
template <typename Encoding, typename Allocator>
double JsonGetDoubleEx2(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, double defaultVal = 0.0,
                        const char* pStreamName = nullptr)
{
    rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
    if (memberIt == value.MemberEnd())
    {
        // Not finding the member is okay.
    }
    else if (!memberIt->value.IsNumber())
    {
        throw XceptionJson(JsonError::ValueIsNotDouble, pName, pStreamName);
    }
    else
    {
        defaultVal = memberIt->value.GetDouble();
    }

    return defaultVal;
};

/// <summary>
/// Returns the float associated with pName, default if pName doesn't exist.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="defaultVal">The value to use if pName doesn't exist.</param>
/// <returns>The float associated with pName, default if pName doesn't exist.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't an float.</exception>
template <typename Encoding, typename Allocator>
float JsonGetFloatEx2(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, float defaultVal = 0.0f,
                      const char* pStreamName = nullptr)
{
    const rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
    if (memberIt == value.MemberEnd())
    {
        // Not finding the member is okay.
    }
    else if (!memberIt->value.IsNumber())
    {
        throw XceptionJson(JsonError::ValueIsNotFloat, pName, pStreamName);
    }
    else
    {
        defaultVal = memberIt->value.GetFloat();
    }

    return defaultVal;
};

/// <summary>
/// Returns the bool associated with pName, or defaultVal if pName doesn't exist.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="defaultVal">The value to use if pName doesn't exist.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>The bool associated with pName, or defaultVal if pName doesn't exist.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't a bool.</exception>
template <typename Encoding, typename Allocator>
bool JsonGetBoolEx2(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, bool defaultVal = false,
                    const char* pStreamName = nullptr)
{
    const rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
    if (memberIt == value.MemberEnd())
    {
        // Not finding the member is okay.
    }
    else if (!memberIt->value.IsBool())
    {
        throw XceptionJson(JsonError::ValueIsNotBool, pName, pStreamName);
    }
    else
    {
        defaultVal = memberIt->value.GetBool();
    }

    return defaultVal;
};

/// <summary>
/// Returns the object associated with pName or nullptr if the object is not present.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the object.</param>
/// <returns>An object associated with pName, nullptr if the object is not present.</returns>
/// <exception cref="XceptionJson">Thrown if the member exists and isn't an object.</exception>
template <typename Encoding, typename Allocator>
const rapidjson::Value* JsonGetObjectEx2(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                         const char* pStreamName = nullptr)
{
    const rapidjson::Value* returnValue{ nullptr };

    auto memberIt = value.FindMember(pName);
    if (memberIt == value.MemberEnd())
    {
        // Not finding the member is okay.
    }
    else if (!memberIt->value.IsObject())
    {
        throw XceptionJson(JsonError::ValueIsNotObject, pName, pStreamName);
    }
    else
    {
        returnValue = &memberIt->value;
    }

    return returnValue;
}

/// <summary>
/// Returns the array associated with pName or nullptr if the array is not present.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the array.</param>
/// <returns>An array associated with pName, nullptr if the array is not present.</returns>
/// <exception cref="XceptionJson">Thrown if the member exists and isn't an array.</exception>
template <typename Encoding, typename Allocator>
const rapidjson::Value* JsonGetArrayEx2(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                        const char* pStreamName = nullptr)
{
    const rapidjson::Value* returnValue{ nullptr };

    const rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
    if (memberIt == value.MemberEnd())
    {
        // Not finding the member is okay.
    }
    else if (!memberIt->value.IsArray())
    {
        throw XceptionJson(JsonError::ValueIsNotArray, pName, pStreamName);
    }
    else
    {
        returnValue = &memberIt->value;
    }

    return returnValue;
}

/// <summary>
/// Returns a JSON string of the nested object associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the object.</param>
/// <returns>A JSON string of the object associated with pName, else an empty string object is not present.</returns>
/// <exception cref="XceptionJson">Thrown if the member exists and isn't an object.</exception>
template <typename Encoding, typename Allocator>
std::string JsonGetNestedEx2(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    rapidjson::StringBuffer buffer;
    auto jObj = JsonGetObjectEx2(value, pName);
    if (jObj && !jObj->ObjectEmpty() && !jObj->IsNull()) // The null check should be superfluous, but here anyway just in case
    {
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        jObj->Accept(writer);
    }
    return buffer.GetString();
}
