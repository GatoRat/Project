// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "JsonReadArray.h"
#include "JsonGetEx2.h"

using namespace std;

static void JsonReadArray(const rapidjson::Value* pArray, const char* pName, vector<uint8_t>& data)
{
    data.reserve(static_cast<size_t>(pArray->Size()));

    for (auto& jItem : pArray->GetArray())
    {
        if (!jItem.IsUint())
        {
            throw XceptionJson(JsonError::ValueIsNotUint, pName);
        }

        data.push_back(static_cast<uint8_t>(jItem.GetUint()));
    }
}

void JsonReadArray(const rapidjson::Value& jObj, const char* pName, vector<uint8_t>& data)
{
    auto pArray = JsonGetArrayEx2(jObj, pName);
    if (pArray && !pArray->IsNull() && !pArray->Empty())
    {
        JsonReadArray(pArray, pName, data);
    }
    else
    {
        data.resize(0);
    }
}

void JsonReadArray(const rapidjson::Value& jObj, const char* pName, optional<vector<uint8_t>>& data)
{
    auto pArray = JsonGetArrayEx2(jObj, pName);
    if (pArray && !pArray->IsNull() && !pArray->Empty())
    {
        if (!data)
        {
            data.emplace();
        }

        JsonReadArray(pArray, pName, *data);
    }
    else
    {
        data.reset();
    }
}
