// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include <ostream>
#include <string>
#include <string_view>
#include <RapidJSON/rapidjson.h>
#include <RapidJSON/document.h>

namespace fs = std::filesystem;

std::string JsonFormatError(const rapidjson::Document& doc, const fs::path& filename, std::string_view function, int line = 0);
std::string JsonFormatError2(const rapidjson::Document& doc, std::string_view parsingWhat, std::string_view function, int line = 0);

std::string JsonFormatError(const rapidjson::Document& doc, const fs::path& filename, const char* pContents, std::string_view function, int line = 0);
std::string JsonFormatError2(const rapidjson::Document& doc, std::string_view parsingWhat, const char* pContents, std::string_view function, int line = 0);

std::ostream& JsonFormatError2(std::ostream& os, const rapidjson::Document& doc, std::string_view parsingWhat, const char* pContents, std::string_view function, int line = 0);
