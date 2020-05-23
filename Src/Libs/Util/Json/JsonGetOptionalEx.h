// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <optional>
#include <RapidJSON/rapidjson.h>
#include <RapidJSON/document.h>
#include "JsonGetEx2.h"
#include "XceptionJson.h"

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
/// Returns a std::optional&lt;const char*&gt; associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>A std::optional&lt;const char*&gt; instance.</returns>
/// <exception cref="JsonException">Thrown if the value exists and isn't a string.</exception>
template <typename Encoding, typename Allocator>
std::optional<const char*> JsonGetOptionalStringEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                                   const char* pStreamName = nullptr)
{
    std::optional<const char*> optionalVal;

    if (!Str::IsNullOrEmpty(pName))
    {
        const rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt == value.MemberEnd())
        {
            // Not finding the member is okay.
        }
        else if (memberIt->value.IsString())
        {
            optionalVal = memberIt->value.GetString();
        }
        else if (!memberIt->value.IsNull())
        {
            throw XceptionJson(JsonError::ValueIsNotString, pName, pStreamName);
        }
    }

    return optionalVal;
};

/// <summary>
/// Returns a std::optional&lt;std::string&gt; associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>A std::optional&lt;std::string&gt; instance.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't a string.</exception>
template <typename Encoding, typename Allocator>
std::optional<std::string> JsonGetOptionalStdStringEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                                      const char* pStreamName = nullptr)
{
    std::optional<std::string> optionalVal;

    const rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
    if (memberIt == value.MemberEnd())
    {
        // Not finding the member is okay.
    }
    else if (memberIt->value.IsString())
    {
        optionalVal.emplace(memberIt->value.GetString());
    }
    else if (!memberIt->value.IsNull())
    {
        throw XceptionJson(JsonError::ValueIsNotString, pName, pStreamName);
    }

    return optionalVal;
};

/// <summary>
/// Returns a std::optional&lt;int&gt; associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>A std::optional&lt;int&gt; instance.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't an int.</exception>
template <typename Encoding, typename Allocator>
std::optional<int> JsonGetOptionalIntEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                        const char* pStreamName = nullptr)
{
    std::optional<int> optionalVal;

    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt == value.MemberEnd())
        {
            // Not finding the member is okay.
        }
        else if (memberIt->value.IsInt())
        {
            optionalVal = memberIt->value.GetInt();
        }
        else if (!memberIt->value.IsNull())
        {
            throw XceptionJson(JsonError::ValueIsNotInt, pName, pStreamName);
        }
    }

    return optionalVal;
};

/// <summary>
/// Returns a std::optional&lt;unsigned&gt; associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>A std::optional&lt;unsigned&gt; instance.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't an unsigned int.</exception>
template <typename Encoding, typename Allocator>
std::optional<unsigned> JsonGetOptionalUintEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                              const char* pStreamName = nullptr)
{
    std::optional<unsigned> optionalVal;

    if (!Str::IsNullOrEmpty(pName))
    {
        const rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt == value.MemberEnd())
        {
            // Not finding the member is okay.
        }
        else if (memberIt->value.IsUint())
        {
            optionalVal = memberIt->value.GetUint();
        }
        else if (!memberIt->value.IsNull())
        {
            throw XceptionJson(JsonError::ValueIsNotUint, pName, pStreamName);
        }
    }

    return optionalVal;
};

/// <summary>
/// Returns a std::optional&lt;int64_t&gt; associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>A std::optional&lt;int64_t&gt; instance.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't an int.</exception>
template <typename Encoding, typename Allocator>
std::optional<int64_t> JsonGetOptionalInt64Ex(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                              const char* pStreamName = nullptr)
{
    std::optional<int64_t> optionalVal;

    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt == value.MemberEnd())
        {
            // Not finding the member is okay.
        }
        else if (memberIt->value.IsInt64())
        {
            optionalVal = memberIt->value.GetInt64();
        }
        else if (!memberIt->value.IsNull())
        {
            throw XceptionJson(JsonError::ValueIsNotInt64, pName, pStreamName);
        }
    }

    return optionalVal;
};

/// <summary>
/// Returns a std::optional&lt;uint64_t&gt; associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>An std::optional&lt;uint64_t&gt; instance.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't an unsigned int.</exception>
template <typename Encoding, typename Allocator>
std::optional<uint64_t> JsonGetOptionalUint64Ex(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                                const char* pStreamName = nullptr)
{
    std::optional<uint64_t> optionalVal;

    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt == value.MemberEnd())
        {
            // Not finding the member is okay.
        }
        else if (memberIt->value.IsUint64())
        {
            optionalVal = memberIt->value.GetUint64();
        }
        else if (!memberIt->value.IsNull())
        {
            throw XceptionJson(JsonError::ValueIsNotUint64, pName, pStreamName);
        }
    }

    return optionalVal;
};

/// <summary>
/// Returns a std::optional&lt;double&gt; associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="strict">If <b>true</b>, the value must be a double, else it can be any number.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>A std::optional&lt;double&gt; instance.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't a double.</exception>
template <typename Encoding, typename Allocator>
std::optional<double> JsonGetOptionalDoubleEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                              bool strict = false, const char* pStreamName = nullptr)
{
    std::optional<double> optionalVal;

    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt == value.MemberEnd())
        {
            // Not finding the member is okay.
        }
        else if ((strict && memberIt->value.IsDouble()) || //-V807
                 (!strict && memberIt->value.IsNumber()))
        {
            optionalVal = memberIt->value.GetDouble();
        }
        else if (!memberIt->value.IsNull())
        {
            throw XceptionJson(JsonError::ValueIsNotDouble, pName, pStreamName);
        }
    }

    return optionalVal;
};

/// <summary>
/// Returns a std::optional&lt;float&gt; associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="strict">If <b>true</b>, the value must be a float, else it can be any number.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>A std::optional&lt;float&gt; instance.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't a float.</exception>
template <typename Encoding, typename Allocator>
std::optional<float> JsonGetOptionalFloatEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                            bool strict = false, const char* pStreamName = nullptr)
{
    std::optional<float> optionalVal;

    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt == value.MemberEnd())
        {
            // Not finding the member is okay.
        }
        else if ((strict && memberIt->value.IsFloat()) || //-V807
                 (!strict && memberIt->value.IsNumber()))
        {
            optionalVal = memberIt->value.GetFloat();
        }
        else if (!memberIt->value.IsNull())
        {
            throw XceptionJson(JsonError::ValueIsNotFloat, pName, pStreamName);
        }
    }

    return optionalVal;
};

/// <summary>
/// Returns a std::optional&lt;bool&gt; associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pStreamName">An optional stream name, for logging and debugging purposes.</param>
/// <returns>A std::optional&lt;bool&gt; instance.</returns>
/// <exception cref="XceptionJson">Thrown if the value exists and isn't a bool.</exception>
template <typename Encoding, typename Allocator>
std::optional<bool> JsonGetOptionalBoolEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName,
                                          const char* pStreamName = nullptr)
{
    std::optional<bool> optionalVal;

    const rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
    if (memberIt == value.MemberEnd())
    {
        // Not finding the member is okay.
    }
    else if (memberIt->value.IsBool())
    {
        optionalVal = memberIt->value.GetBool();
    }
    else if (!memberIt->value.IsNull())
    {
        throw XceptionJson(JsonError::ValueIsNotBool, pName, pStreamName);
    }

    return optionalVal;
};

/// <summary>
/// Returns a JSON string of the nested object associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the object.</param>
/// <returns>A JSON string of the object associated with pName, else an empty string object is not present.</returns>
/// <exception cref="XceptionJson">Thrown if the member exists and isn't an object.</exception>
template <typename Encoding, typename Allocator>
std::optional<std::string> JsonGetOptionalNestedEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    std::optional<std::string> optionalVal;

    rapidjson::StringBuffer buffer;
    auto jObj = JsonGetObjectEx2(value, pName);
    if (jObj && !jObj->ObjectEmpty() && !jObj->IsNull()) // The null check should be superfluous, but here anyway just in case
    {
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        jObj->Accept(writer);
        optionalVal = move(buffer.GetString());
    }

    return optionalVal;
}
