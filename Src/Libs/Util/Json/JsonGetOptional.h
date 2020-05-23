// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <optional>
#include <string>
#include <RapidJSON/rapidjson.h>
#include <RapidJSON/document.h>

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
/// <returns>A std::optional&lt;const char*&gt; instance.</returns>
template <typename Encoding, typename Allocator>
std::optional<const char*> JsonGetOptionalString(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    std::optional<const char*> optionalVal;

    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsString())
        {
            optionalVal = memberIt->value.GetString();
        }
    }

    return optionalVal;
};

/// <summary>
/// Returns a std::optional&lt;std::string&gt; associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <returns>A std::optional&lt;std::string&gt; instance.</returns>
template <typename Encoding, typename Allocator>
std::optional<std::string> JsonGetOptionalStdString(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    std::optional<std::string> optionalVal;

    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsString())
        {
            optionalVal = memberIt->value.GetString();
        }
    }

    return optionalVal;
};

/// <summary>
/// Returns a std::optional&lt;int&gt; associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <returns>A std::optional&lt;int&gt; instance.</returns>
template <typename Encoding, typename Allocator>
std::optional<int> JsonGetOptionalInt(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    std::optional<int> optionalVal;

    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsInt())
        {
            optionalVal = memberIt->value.GetInt();
        }
    }

    return optionalVal;
};

/// <summary>
/// Returns a std::optional&lt;unsigned&gt; associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <returns>A std::optional&lt;unsigned&gt; instance.</returns>
template <typename Encoding, typename Allocator>
std::optional<unsigned> JsonGetOptionalUint(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    std::optional<unsigned> optionalVal;

    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && !memberIt->value.IsUint())
        {
            optionalVal = memberIt->value.GetUint();
        }
    }

    return optionalVal;
};

/// <summary>
/// Returns a std::optional&lt;int64_t&gt; associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <returns>A std::optional&lt;int64_t&gt; instance.</returns>
template <typename Encoding, typename Allocator>
std::optional<int64_t> JsonGetOptionalInt64(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    std::optional<int64_t> optionalVal;

    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsInt64())
        {
            optionalVal = memberIt->value.GetInt64();
        }
    }

    return optionalVal;
};

/// <summary>
/// Returns a std::optional&lt;uint64_t&gt; associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <returns>An std::optional&lt;uint64_t&gt; instance.</returns>
template <typename Encoding, typename Allocator>
std::optional<uint64_t> JsonGetOptionalUint64(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    std::optional<uint64_t> optionalVal;

    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsUint64())
        {
            optionalVal = memberIt->value.GetUint64();
        }
    }

    return optionalVal;
};

/// <summary>
/// Returns a std::optional&lt;double&gt; associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <returns>A std::optional&lt;double&gt; instance.</returns>
template <typename Encoding, typename Allocator>
std::optional<double> JsonGetOptionalDouble(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    std::optional<double> optionalVal;

    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsNumber())
        {
            optionalVal = memberIt->value.GetDouble();
        }
    }

    return optionalVal;
};

/// <summary>
/// Returns a std::optional&lt;float&gt; associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <returns>A std::optional&lt;float&gt; instance.</returns>
template <typename Encoding, typename Allocator>
std::optional<float> JsonGetOptionalFloat(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    std::optional<float> optionalVal;

    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsNumber())
        {
            optionalVal = memberIt->value.GetFloat();
        }
    }

    return optionalVal;
};

/// <summary>
/// Returns a std::optional&lt;bool&gt; associated with pName.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <returns>A std::optional&lt;bool&gt; instance.</returns>
template <typename Encoding, typename Allocator>
std::optional<bool> JsonGetOptionalBool(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    std::optional<bool> optionalVal;

    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsBool())
        {
            optionalVal = memberIt->value.GetBool();
        }
    }

    return optionalVal;
};

/// <summary>
/// Returns an array object associated with pName, nullptr if the array object is not present.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <returns>An array object associated with pName, nullptr if the array object is not present.</returns>
template <typename Encoding, typename Allocator>
const rapidjson::Value* JsonGetOptionalArrayEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    const rapidjson::Value* returnValue{ nullptr };

    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsArray())
        {
            returnValue = &memberIt->value;
        }
    }

    return returnValue;
}
