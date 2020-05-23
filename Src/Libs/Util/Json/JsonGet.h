// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <string>
#include <RapidJSON/rapidjson.h>
#include <RapidJSON/document.h>
#include "../String/Str.h"

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
/// Returns the string associated with pName, or pDefaultStr if pName doesn't exist or isn't a string.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pDefaultStr">The default string.</param>
/// <returns>The string associated with pName, or pDefaultStr if pName doesn't exist or isn't a string.</returns>
template <typename Encoding, typename Allocator>
const char* JsonGetString(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, const char* pDefaultStr = "")
{
    if (!Str::IsNullOrEmpty(pName))
    {
        const auto memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsString())
        {
            pDefaultStr = memberIt->value.GetString();
        }
    }
    return pDefaultStr;
}

/// <summary>
/// Returns the string view associated with pName, or defaultStr if pName doesn't exist or isn't a string.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pDefaultStr">The default string.</param>
/// <returns>The string view associated with pName, or defaultStr if pName doesn't exist or isn't a string.</returns>
template <typename Encoding, typename Allocator>
std::string_view JsonGetStringView(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, std::string_view defaultStr = "")
{
    if (!Str::IsNullOrEmpty(pName))
    {
        const auto memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsString())
        {
            defaultStr = memberIt->value.GetString();
        }
    }
    return defaultStr;
}

/// <summary>
/// Returns the string associated with pName, or pDefaultStr if pName doesn't exist or isn't a string.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="pDefaultStr">The default string.</param>
/// <returns>The string associated with pName, or pDefaultStr if pName doesn't exist or isn't a string.</returns>
template <typename Encoding, typename Allocator>
std::string JsonGetStdString(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, const char* pDefaultStr = "")
{
    pDefaultStr = JsonGetString(value, pName, pDefaultStr);
    return std::string(pDefaultStr ? pDefaultStr : "");
}

/// <summary>
/// Returns the int associated with pName, or defaultVal if pName doesn't exist or isn't an int.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="defaultVal">The default value.</param>
/// <returns>The int associated with pName, or defaultVal if pName doesn't exist or isn't an int.</returns>
template <typename Encoding, typename Allocator>
int JsonGetInt(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, int defaultVal = 0)
{
    if (!Str::IsNullOrEmpty(pName))
    {
        const rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsInt())
        {
            defaultVal = memberIt->value.GetInt();
        }
    }
    return defaultVal;
}

/// <summary>
/// Returns the unsigned int associated with pName, or defaultVal if pName doesn't exist or isn't an unsigned int.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="defaultVal">The default value.</param>
/// <returns>The unsigned int associated with pName, or defaultVal if pName doesn't exist or isn't an unsigned int.</returns>
template <typename Encoding, typename Allocator>
unsigned JsonGetUint(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, unsigned defaultVal = 0)
{
    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && !memberIt->value.IsUint())
        {
            defaultVal = memberIt->value.GetUint();
        }
    }
    return defaultVal;
}

/// <summary>
/// Returns the int64_t associated with pName, or defaultVal if pName doesn't exist or isn't an int64_t.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="defaultVal">The default value.</param>
/// <returns>The int64_t associated with pName, or defaultVal if pName doesn't exist or isn't an int64_t.</returns>
template <typename Encoding, typename Allocator>
int64_t JsonGetInt64(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, int64_t defaultVal = 0)
{
    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsInt64())
        {
            defaultVal = memberIt->value.GetInt64();
        }
    }
    return defaultVal;
};

/// <summary>
/// Returns the uint64_t associated with pName, or defaultVal if pName doesn't exist or isn't an uint64_t.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="defaultVal">The default value.</param>
/// <returns>The uint64_t associated with pName, or defaultVal if pName doesn't exist or isn't an uint64_t.</returns>
template <typename Encoding, typename Allocator>
uint64_t JsonGetUint64(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, uint64_t defaultVal = 0)
{
    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsUint64())
        {
            defaultVal = memberIt->value.GetUint64();
        }
    }
    return defaultVal;
};

/// <summary>
/// Returns the double associated with pName, or defaultVal if pName doesn't exist or isn't a double.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="defaultVal">The default value.</param>
/// <returns>The double associated with pName, or defaultVal if pName doesn't exist or isn't a double.</returns>
template <typename Encoding, typename Allocator>
double JsonGetDouble(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, double defaultVal = 0.0)
{
    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsNumber())
        {
            defaultVal = memberIt->value.GetDouble();
        }
    }
    return defaultVal;
};

/// <summary>
/// Returns the float associated with pName, or defaultVal if pName doesn't exist or isn't a float.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="defaultVal">The default value.</param>
/// <returns>The float associated with pName, or defaultVal if pName doesn't exist or isn't a float.</returns>
template <typename Encoding, typename Allocator>
float JsonGetFloat(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, float defaultVal = 0.0f)
{
    if (!Str::IsNullOrEmpty(pName))
    {
        auto memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsNumber())
        {
            defaultVal = memberIt->value.GetFloat();
        }
    }

    return defaultVal;
};

/// <summary>
/// Returns the bool associated with pName, or defaultVal if pName doesn't exist or isn't a bool.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <param name="defaultVal">The default value.</param>
/// <returns>The bool associated with pName, or defaultVal if pName doesn't exist or isn't a bool.</returns>
template <typename Encoding, typename Allocator>
bool JsonGetBool(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName, bool defaultVal = false)
{
    if (!Str::IsNullOrEmpty(pName))
    {
        const rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsBool())
        {
            defaultVal = memberIt->value.GetBool();
        }
    }

    return defaultVal;
};

/// <summary>
/// Returns an array object associated with pName, nullptr if the array object is not present.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <returns>An array object associated with pName, nullptr if the array object is not present.</returns>
template <typename Encoding, typename Allocator>
const rapidjson::Value* JsonGetArray(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    const rapidjson::Value* pReturnValue{ nullptr };

    if (!Str::IsNullOrEmpty(pName))
    {
        rapidjson::Value::ConstMemberIterator memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsArray())
        {
            pReturnValue = &memberIt->value;
        }
    }

    return pReturnValue;
}

/// <summary>
/// Returns an object associated with pName, nullptr if the object is not present.
/// </summary>
/// <param name="value">A rapidjson::GenericValue.</param>
/// <param name="pName">The name of the value.</param>
/// <returns>An object associated with pName, nullptr if the object is not present.</returns>
template <typename Encoding, typename Allocator>
const rapidjson::Value* JsonGetObject(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    const rapidjson::Value* pReturnValue{ nullptr };

    if (!Str::IsNullOrEmpty(pName))
    {
        auto memberIt = value.FindMember(pName);
        if (memberIt != value.MemberEnd() && memberIt->value.IsObject())
        {
            pReturnValue = &memberIt->value;
        }
    }

    return pReturnValue;
}
