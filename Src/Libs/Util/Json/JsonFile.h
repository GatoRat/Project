// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include <string_view>
#include <RapidJSON/rapidjson.h>
#include <RapidJSON/document.h>

namespace fs = std::filesystem;

void JsonReadFile(const fs::path& pathname, rapidjson::Document& doc, bool parseIfEmpty = true);
/**
    @remarks Returned string must remain valid for the life of \p doc.
*/
std::string JsonReadFileInsitu(const fs::path& pathname, rapidjson::Document& doc, bool parseIfEmpty = true);

void JsonPrettyWriteFile(rapidjson::Document& doc, const fs::path& pathname, bool useTabs = false, unsigned spacesPerTab = 2);
void JsonWriteFile(const char* pJsonStr, const fs::path& pathname, bool prettyWrite = true, bool useTabs = false, unsigned spacesPerTab = 2);
