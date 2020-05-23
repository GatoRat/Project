// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <RapidJSON/rapidjson.h>
#include <RapidJSON/document.h>
#include "XceptionJson.h"
#include "../Time/TimeStamp.h"

template <typename Encoding, typename Allocator>
std::optional<TimeStamp> JsonGetOptionalTimeStampEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    std::optional<TimeStamp> optionalVal;

    if (!Str::IsNullOrEmpty(pName))
    {
        const auto memberIt = value.FindMember(pName);
        if (memberIt == value.MemberEnd())
        {
            // Not finding the member is okay.
        }
        else if (memberIt->value.IsInt64())
        {
            optionalVal.emplace(memberIt->value.GetInt64());
        }
        else if (memberIt->value.IsString())
        {
            bool error = false;
            optionalVal.emplace(memberIt->value.GetString(), false, nullptr, &error);
            if (error) //-V547
            {
                throw XceptionJson(JsonError::ValueInvalid, pName);
            }
        }
        else if (!memberIt->value.IsNull())
        {
            throw XceptionJson(JsonError::ValueInvalid, pName);
        }
    }

    return optionalVal;
};

template <typename Encoding, typename Allocator>
TimeStamp JsonGetTimeStampEx(const rapidjson::GenericValue<Encoding, Allocator>& value, const char* pName)
{
    auto val = JsonGetOptionalTimeStampEx(value, pName);
    if (!val)
    {
        throw XceptionJson(JsonError::ValueInvalid, pName);
    }
    return val.value();
}
