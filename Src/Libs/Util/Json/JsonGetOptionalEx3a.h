// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <RapidJSON/rapidjson.h>
#include <RapidJSON/document.h>
#include "XceptionJson.h"

namespace JsonOptional
{
    enum class Result
    {
        Missing,
        IsNull,
        Valid
    };

    template<class T>
    struct Value
    {
        T      value;
        Result result;

        bool isMissing() const  { return result == Result::Missing; }
        bool isNull() const     { return result == Result::IsNull;  }
        bool isValid() const    { return result == Result::Valid;   }
    };

    Value<const rapidjson::Value*> GetValue  (const rapidjson::Value& jValue, const char* pName);
    Value<const rapidjson::Value*> GetValueEx(const rapidjson::Value& jValue, const char* pName, bool canBeNull);

    Value<const char*> GetStrEx  (const rapidjson::Value& jValue, const char* pName, bool canBeNull);
    Value<int>         GetIntEx  (const rapidjson::Value& jValue, const char* pName, bool canBeNull);
    Value<int64_t>     GetInt64Ex(const rapidjson::Value& jValue, const char* pName, bool canBeNull);
    Value<bool>        GetBoolEx (const rapidjson::Value& jValue, const char* pName, bool canBeNull);

    Result Get  (const rapidjson::Value& jValue, const char* pName, const rapidjson::Value*& pJsonValue);
    Result GetEx(const rapidjson::Value& jValue, const char* pName, bool canBeNull, const rapidjson::Value*& pJsonValue);

    Result GetEx(const rapidjson::Value& jValue, const char* pName, bool canBeNull, const char*& value);
    Result GetEx(const rapidjson::Value& jValue, const char* pName, bool canBeNull, int&         value);
    Result GetEx(const rapidjson::Value& jValue, const char* pName, bool canBeNull, int64_t&     value);
    Result GetEx(const rapidjson::Value& jValue, const char* pName, bool canBeNull, bool&        value);

#if __has_include(<qstring.h>)

    inline
    Value<QString> GetQStrEx(const rapidjson::Value& jValue, const char* pName, bool canBeNull)
    {
        Value<QString> result;

        Value<const char*> value = GetStrEx(jValue, pName, canBeNull);
        if (value.isValid())
        {
            result.value = value.value;
        }
        result.result = value.result;

        return result;
    }

    inline
    Result GetEx(const rapidjson::Value& jValue, const char* pName, bool canBeNull, QString& value)
    {
        const char* pStr = nullptr;
        Result result = GetEx(jValue, pName, canBeNull, pStr);
        if (result != Result::Missing)
        {
            value = pStr;
        }
        return result;
    }
#endif

    template<class T>
    bool GetEx2(const rapidjson::Value& jValue, const char* pName, bool canBeNull, std::optional<T>& value)
    {
        bool result = false;

        T tmpValue;
        switch (JsonOptional::GetEx(jValue, pName, canBeNull, tmpValue)) //-V719
        {
            case Result::Valid:
                value.emplace(tmpValue);
                result = true;
                break;

            case Result::IsNull:
                value.reset();
                result = true;
                break;
        }

        return result;
    };

    bool GetEx2(const rapidjson::Value& jValue, const char* pName, bool canBeNull, std::optional<std::string>& value);
}