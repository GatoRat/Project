// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <RapidJSON/rapidjson.h>
#include <RapidJSON/document.h>

class JsonPath
{
public:
    JsonPath() = default;
    explicit JsonPath(std::string_view jsonPath);
    JsonPath(std::string_view jsonPath1, std::string_view jsonPath2);
    JsonPath(const JsonPath& jsonPath1, std::string_view jsonPath2);

    bool isEmpty() const                                { return _tokens.empty(); }
    size_t size() const                                 { return _tokens.size(); }

    std::string toPointer(const char* pName = nullptr) const;

    struct Filename
    {
        std::string filename;
        bool        isPlural = false;
    };

    Filename toFilename() const;   // This is very Skin centric
    Filename toFilename(const char* pPath) const;   // This is very Skin centric

    static constexpr size_t npos = SIZE_MAX;
    size_t find(std::string_view token) const;

    std::string_view operator[](size_t index) const;

    char front() const;

    JsonPath& append(std::string_view jsonPath);
    JsonPath& operator+=(std::string_view jsonPath)     { return append(jsonPath); }

    JsonPath& append(const JsonPath& jsonPath);
    JsonPath& operator+=(const JsonPath& jsonPath)      { return append(jsonPath); }

    JsonPath& replaceEnd(std::string_view jsonPath);
    JsonPath& removeEnd();
    JsonPath& removeAfter(size_t index);
    JsonPath& removeAfter(std::string_view token);

    JsonPath& replaceEnd(const JsonPath& jsonPath);

    static std::string_view GetLastToken(std::string_view jsonPath);

    struct Result
    {
        const rapidjson::Value* pValue = nullptr;
        bool haltSearch = false;

        bool isValid() const                    { return !haltSearch && pValue; }
        bool keepSearching() const              { return !haltSearch && !pValue; }
        void reset(bool haltSearch_ = false)    { pValue = nullptr; haltSearch = haltSearch_; }
    };

    Result get(const rapidjson::Value* pRoot) const;
    Result search(const rapidjson::Value* pRoot, const char* pName, bool allowRoot = true) const;

private:
    Result get(const rapidjson::Value* pRoot, std::vector<const rapidjson::Value*>* pValues) const;

private:
    std::vector<std::string> _tokens;
};
