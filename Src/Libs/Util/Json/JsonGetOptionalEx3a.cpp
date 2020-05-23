// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "JsonGetOptionalEx3a.h"

using namespace std;

namespace JsonOptional
{
    Value<const rapidjson::Value*> GetValue(const rapidjson::Value& jValue, const char* pName)
    {
        Value<const rapidjson::Value*> result;

        auto memberIt = jValue.FindMember(pName);
        if (memberIt != jValue.MemberEnd())
        {
            result.value  = &memberIt->value;
            result.result = result.value->IsNull() ? Result::IsNull : Result::Valid;
        }
        else
        {
            result.value  = nullptr;
            result.result = Result::Missing;
        }

        return result;
    };

    Value<const rapidjson::Value*> GetValueEx(const rapidjson::Value& jValue, const char* pName, bool canBeNull)
    {
        auto result = GetValue(jValue, pName);
        if (result.result == Result::IsNull && !canBeNull)
        {
            throw XceptionJson(JsonError::ValueIsNull, pName);
        }
        return result;
    };

    Value<const char*> GetStrEx(const rapidjson::Value& jValue, const char* pName, bool canBeNull)
    {
        Value<const char*> result;

        auto value = GetValueEx(jValue, pName, canBeNull);
        if (value.result == Result::Valid)
        {
            if (value.value->IsString())
            {
                result.value = value.value->GetString();
            }
            else
            {
                throw XceptionJson(JsonError::ValueIsNotString, pName);
            }
        }

        result.result = value.result;
        return result;
    }

    Value<int> GetIntEx(const rapidjson::Value& jValue, const char* pName, bool canBeNull)
    {
        Value<int> result;

        auto value = GetValueEx(jValue, pName, canBeNull);
        if (value.result == Result::Valid)
        {
            if (value.value->IsString())
            {
                result.value = value.value->GetInt();
            }
            else
            {
                throw XceptionJson(JsonError::ValueIsNotInt64, pName);
            }
        }

        result.result = value.result;
        return result;
    }

    Value<int64_t> GetInt64Ex(const rapidjson::Value& jValue, const char* pName, bool canBeNull)
    {
        Value<int64_t> result;

        auto value = GetValueEx(jValue, pName, canBeNull);
        if (value.result == Result::Valid)
        {
            if (value.value->IsString())
            {
                result.value = value.value->GetInt64();
            }
            else
            {
                throw XceptionJson(JsonError::ValueIsNotInt64, pName);
            }
        }

        result.result = value.result;
        return result;
    }

    Value<bool> GetBoolEx(const rapidjson::Value& jValue, const char* pName, bool canBeNull)
    {
        Value<bool> result;

        auto value = GetValueEx(jValue, pName, canBeNull);
        if (value.result == Result::Valid)
        {
            if (value.value->IsString())
            {
                result.value = value.value->GetBool();
            }
            else
            {
                throw XceptionJson(JsonError::ValueIsNotBool, pName);
            }
        }

        result.result = value.result;
        return result;
    }

    ///////////////////////////////////////////////////////////////////////

    Result Get(const rapidjson::Value& jValue, const char* pName, const rapidjson::Value*& pJsonValue)
    {
        Result result;

        auto memberIt = jValue.FindMember(pName);
        if (memberIt != jValue.MemberEnd())
        {
            pJsonValue = &memberIt->value;
            result = pJsonValue->IsNull() ? Result::IsNull : Result::Valid;
        }
        else
        {
            pJsonValue = nullptr;
            result = Result::Missing;
        }

        return result;
    };

    Result GetEx(const rapidjson::Value& jValue, const char* pName, bool canBeNull, const rapidjson::Value*& pJsonValue)
    {
        auto result = Get(jValue, pName, pJsonValue);
        if (result == Result::IsNull && !canBeNull)
        {
            throw XceptionJson(JsonError::ValueIsNull, pName);
        }
        return result;
    };

    Result GetEx(const rapidjson::Value& jValue, const char* pName, bool canBeNull, const char*& value)
    {
        const rapidjson::Value* pJsonValue;
        auto result = GetEx(jValue, pName, canBeNull, pJsonValue);
        if (result == Result::Valid)
        {
            if (pJsonValue->IsString())
            {
                value = pJsonValue->GetString();
            }
            else
            {
                throw XceptionJson(JsonError::ValueIsNotString, pName);
            }
        }

        return result;
    }

    Result GetEx(const rapidjson::Value& jValue, const char* pName, bool canBeNull, int& value)
    {
        const rapidjson::Value* pJsonValue;
        auto result = GetEx(jValue, pName, canBeNull, pJsonValue);
        if (result == Result::Valid)
        {
            if (pJsonValue->IsInt())
            {
                value = pJsonValue->GetInt();
            }
            else
            {
                throw XceptionJson(JsonError::ValueIsNotInt, pName);
            }
        }

        return result;
    }

    Result GetEx(const rapidjson::Value& jValue, const char* pName, bool canBeNull, int64_t& value)
    {
        const rapidjson::Value* pJsonValue;
        auto result = GetEx(jValue, pName, canBeNull, pJsonValue);
        if (result == Result::Valid)
        {
            if (pJsonValue->IsInt64())
            {
                value = pJsonValue->GetInt64();
            }
            else
            {
                throw XceptionJson(JsonError::ValueIsNotInt64, pName);
            }
        }

        return result;
    }

    Result GetEx(const rapidjson::Value& jValue, const char* pName, bool canBeNull, bool& value)
    {
        const rapidjson::Value* pJsonValue;
        auto result = GetEx(jValue, pName, canBeNull, pJsonValue);
        if (result == Result::Valid)
        {
            if (pJsonValue->IsBool())
            {
                value = pJsonValue->GetBool();
            }
            else
            {
                throw XceptionJson(JsonError::ValueIsNotBool, pName);
            }
        }

        return result;
    }

    bool GetEx2(const rapidjson::Value& jValue, const char* pName, bool canBeNull, optional<string>& value)
    {
        bool result = false;

        const char* pValue;
        switch (GetEx(jValue, pName, canBeNull, pValue)) //-V719
        {
            case Result::Valid:
                value.emplace(pValue);
                result = true;
                break;

            case Result::IsNull:
                value.reset();
                result = true;
                break;
        }

        return result;
    };
}