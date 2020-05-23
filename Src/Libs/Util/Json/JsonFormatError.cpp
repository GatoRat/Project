// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "JsonFormatError.h"
#include <sstream>
#include <RapidJSON/error/en.h>
#include "Util/String/Str.h"

using namespace std;

string JsonFormatError(const rapidjson::Document& doc, const fs::path& filename, string_view function, int line)
{
    return JsonFormatError(doc, filename, nullptr, function, line);
}

string JsonFormatError2(const rapidjson::Document& doc, string_view parsingWhat, string_view function, int line)
{
    return JsonFormatError2(doc, parsingWhat, nullptr, function, line);
}

string JsonFormatError(const rapidjson::Document& doc, const fs::path& filename, const char* pContents, string_view function, int line)
{
    string parsingWhat;
    parsingWhat += '"';
    parsingWhat += filename.string();
    parsingWhat += '"';
    return JsonFormatError2(doc, parsingWhat, pContents, function, line);
}

string JsonFormatError2(const rapidjson::Document& doc, string_view parsingWhat, const char* pContents, string_view function, int line)
{
    stringstream ss;
    JsonFormatError2(ss, doc, parsingWhat, pContents, function, line);
    return ss.str();
}

ostream& JsonFormatError2(ostream& os, const rapidjson::Document& doc, string_view parsingWhat, const char* pContents, string_view function, int line)
{
    os << "Error: Parsing " << parsingWhat;
    if (pContents)
    {
        const Str::LineColumn lineColumn(pContents, doc.GetErrorOffset());
        if (lineColumn.line > 1)
        {
            os << ": Line " << lineColumn.line << " Column " << lineColumn.column; //-V128
        }
        else
        {
            pContents = nullptr;
        }
    }

    if (!pContents)
    {
        os << ": Offset " << doc.GetErrorOffset();
    }

    os << ": " << rapidjson::GetParseError_En(doc.GetParseError());

    if (!function.empty())
    {
        os << " [" << function;
        if (line > 0)
        {
            os << ", " << line;
        }
        os << ']';
    }

    return os;
}
