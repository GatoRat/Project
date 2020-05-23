// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "SpriteSheet.h"
#include "../Json/JsonGetEx.h"
#include "../Json/JsonFormatError.h"
#include "../Json/JsonFile.h"
#include "../String/Str.h"

using namespace std;

namespace Graphics
{
//    constexpr int TabSpaces{ 2 };

    SheetDef::SheetDef(const fs::path& sheetName)
    {
        fs::path filename = sheetName;
        if (!Str::EndsWith(filename.c_str(), L".json"))
        {
            filename += L".json";
        }

        rapidjson::Document doc;
        string contents = JsonReadFileInsitu(sheetName, doc); //-V808
        read(doc, filename);
    }

    SheetDef::SheetDef(string sheetJson)
    {
        rapidjson::Document doc;
        if (doc.ParseInsitu(sheetJson.data()).HasParseError())
        {
            const string errStr = JsonFormatError2(doc, "memory", sheetJson.c_str(), __FUNCTION__, __LINE__);
            throw Xception(errStr);
        }

        read(doc, "memory");
    }

    void SheetDef::read(const rapidjson::Value& jObj, const fs::path& streamName)
    {
        try
        {
            const auto& jFrames = JsonGetArrayEx(jObj, "frames");
            for (auto& jsonFrame : jFrames.GetArray())
            {
                _frames.emplace_back(jsonFrame);
            }

            sort(_frames.begin(), _frames.end(), [](const Frame& frame1, const Frame& frame2)
            {
                return frame1.filename < frame2.filename;
            });
        }
        catch (XceptionJson& e)
        {
            e.setPath(streamName);
            throw;
        }
    }

    SheetDef::Frame::Frame(const rapidjson::Value& jsonFrame)
        : filename        { JsonGetStdStringEx(jsonFrame, "filename"        ) }
        , frameRect       {                    jsonFrame, "frame"             }
        , spriteSourceRect{                    jsonFrame, "spriteSourceSize"  }
        , sourceSize      {                    jsonFrame, "sourceSize"        }
        , pivot           {                    jsonFrame, "pivot"             }
        , rotated         { JsonGetBoolEx     (jsonFrame, "rotated"         ) }
        , trimmed         { JsonGetBoolEx     (jsonFrame, "trimmmed"        ) }
    {
    }

    void SheetDef::Frame::write(JsonWriter& writer) const
    {
        JsonWriter::Object jObj(writer);

        writer.keyVal("filename", filename);

        frameRect.write       (writer, "frame"           , true);
        spriteSourceRect.write(writer, "spriteSourceSize", true);
        sourceSize.write      (writer, "sourceSize"      , true);
        pivot.write           (writer, "pivot");

        writer.keyVal("rotated" , rotated);
        writer.keyVal("trimmmed", trimmed);
    }

    void SheetDef::write(const fs::path& pathname, bool prettyWrite) const
    {
        string json = writeJson();
        JsonWriteFile(json.c_str(), pathname, prettyWrite);
    }

    string SheetDef::writeJson() const
    {
        JsonWriter writer;
        {
            JsonWriter::Array jArray("frames", writer);

            for (auto& frame : _frames)
            {
                frame.write(writer);
            }
        }
        return writer.getString();
    }
}
