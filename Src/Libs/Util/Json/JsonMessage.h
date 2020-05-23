#pragma once

#include <RapidJSON/rapidjson.h>
#include <RapidJSON/document.h>

// This does not implement move or swap, so don't use either
class JsonMessage : public rapidjson::Document
{
public:
    explicit JsonMessage(std::string& data)
        : _data{ move(data) }
    {
    }

    GenericDocument& ParseInsitu()
    {
        return rapidjson::Document::ParseInsitu(reinterpret_cast<char*>(_data.data()));
    }

private:
    std::string _data;
};

