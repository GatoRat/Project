// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <RapidJSON/rapidjson.h>
#include <RapidJSON/document.h>
#include "Graphics.h"

namespace fs = std::filesystem;

namespace Graphics
{
    class SheetDef
    {
    public:
        struct Frame
        {
            std::string filename;
            Rect        frameRect;
            Rect        spriteSourceRect;
            Size        sourceSize;
            PositionF   pivot;
            bool        rotated = false;
            bool        trimmed = false;

            Frame(const rapidjson::Value& jsonVal);

            void write(JsonWriter& writer) const;
        };

    public:
        SheetDef() = default;
        explicit SheetDef(const fs::path& sheetName);
        // Unless caller wants to retain string, move the string on call
        explicit SheetDef(std::string jsonSheet);
        ~SheetDef() = default;

        void write(const fs::path& pathname_, bool prettyWrite = true) const;
        std::string writeJson() const;

        const Frame& getFrame(size_t index) const       { return _frames[index]; }
        size_t getNumFrames() const                     { return _frames.size(); }

    private:
        void read(const rapidjson::Value& jObj, const fs::path& streamName);

    public:
        std::vector<Frame> _frames;
    };
}