// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <optional>
#include <vector>
#include <RapidJSON/rapidjson.h>
#include <RapidJSON/document.h>
#include "Util/Json/JsonWriter.h"

void JsonReadArray(const rapidjson::Value& jObj, const char* pName, std::vector<uint8_t>& data);
void JsonReadArray(const rapidjson::Value& jObj, const char* pName, std::optional<std::vector<uint8_t>>& data);
