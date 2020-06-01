// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "JsonFile.h"
#include <RapidJSON/rapidjson.h>
#include <RapidJSON/filewritestream.h>
#include <RapidJSON/prettywriter.h>
#include "../File/FastFile.h"
#include "../File/StdioFile.h"
#include "../Misc/Defines.Misc.h"
#include "../String/ASCII.h"
#include "JsonFormatError.h"
#include "XceptionJson.h"

using namespace std;

void JsonReadFile(const fs::path& pathname, rapidjson::Document& doc, bool parseIfEmpty)
{
    auto contents = FastReadFileStr(pathname);
    if (!contents.empty() || parseIfEmpty)
    {
        if (doc.Parse(contents).HasParseError())
        {
            const string errStr = JsonFormatError(doc, pathname, contents.c_str(), __FUNCTION__, __LINE__);
            throw Xception(errStr);
        }
    }
}

string JsonReadFileInsitu(const fs::path& pathname, rapidjson::Document& doc, bool parseIfEmpty)
{
    auto contents = FastReadFileStr(pathname);
    if (!contents.empty() || parseIfEmpty)
    {
        if (doc.ParseInsitu(contents.data()).HasParseError())
        {
            const string errStr = JsonFormatError(doc, pathname, contents.c_str(), __FUNCTION__, __LINE__);
            throw Xception(errStr);
        }
    }
    return contents;
}

void JsonPrettyWriteFile(const rapidjson::Document& doc, const fs::path& pathname, bool useTabs, unsigned spacesPerTab)
{
    StdioFile file(pathname, L"wb");

    string writeBuffer;
    writeBuffer.resize(64 * ONE_KILOBYTE);

    rapidjson::FileWriteStream os(file, writeBuffer.data(), writeBuffer.size());
    rapidjson::PrettyWriter<rapidjson::FileWriteStream> prettyWriter(os);
    prettyWriter.SetIndent(useTabs ? ASCII::TAB : ASCII::Space, spacesPerTab);
    if (!doc.Accept(prettyWriter))
    {
        throw XceptionJson(pathname, JsonError::Code::UnknownWriteError);
    }
}

void JsonWriteFile(const char* pJsonStr, const fs::path& pathname, bool prettyWrite, bool useTabs, unsigned spacesPerTab)
{
    if (pJsonStr && *pJsonStr)
    {
        if (prettyWrite)
        {
            rapidjson::Document doc;
            if (doc.Parse(pJsonStr).HasParseError())
            {
                const string errStr = JsonFormatError(doc, pathname, pJsonStr, __FUNCTION__, __LINE__);
                throw XceptionFile(pathname, 0, errStr);
            }

            JsonPrettyWriteFile(doc, pathname, useTabs, spacesPerTab);
        }
        else
        {
            FastWriteFile(pathname, pJsonStr, static_cast<uint32_t>(strlen(pJsonStr)));
        }
    }
    else
    {
        error_code ec;
        fs::remove(pathname, ec);
    }
}
