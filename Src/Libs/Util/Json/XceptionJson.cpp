// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "XceptionJson.h"
#include <map>
#include <RapidJSON/error/en.h>

using namespace std;
using namespace rapidjson;

static const char* pUnknownError = "Unknown error";

static map<JsonError::Code, const char*> jsonErrorStrings =
{
    { JsonError::FileNotFound      , "File not found"                },
    { JsonError::NullOrEmpty       , "Json string is null or empty"  },
    { JsonError::FormatError       , "Format error"                  },
    { JsonError::MemberNotFound    , "Member not found"              },
    { JsonError::StringValueEmpty  , "String value is null or empty" },
    { JsonError::ValueIsNotString  , "Value is not a string"         },
    { JsonError::ValueIsNotInt     , "Value is not an Int"           },
    { JsonError::ValueIsNotUint    , "Value is not a UInt"           },
    { JsonError::ValueIsNotInt64   , "Value is not an Int64"         },
    { JsonError::ValueIsNotUint64  , "Value is not a UInt64"         },
    { JsonError::ValueIsNotFloat   , "Value is not a float"          },
    { JsonError::ValueIsNotDouble  , "Value is not a double"         },
    { JsonError::ValueIsNotNumber  , "Value is not a number"         },
    { JsonError::ValueIsNotBool    , "Value is not a bool"           },
    { JsonError::ValueIsNotArray   , "Value is not an array"         },
    { JsonError::ValueIsNotObject  , "Value is not an object"        },
    { JsonError::ValueInvalid      , "Value is invalid"              },
    { JsonError::ValueOutOfRange   , "Value is out-of-range"         },
    { JsonError::ValueIsNull       , "Value is null"                 },
    { JsonError::UnknownWriteError , "Unknown write error"           },

    { JsonError::UnknownError      , pUnknownError                   }
};

const char* JsonError::GetString(Code errorCode, bool returnNullOnError)
{
    const char* pErrorStr = nullptr;

    if (errorCode == Code::Success)
    {
        pErrorStr = "";
    }
    else
    {
        const auto found = jsonErrorStrings.find(errorCode);
        if (found != jsonErrorStrings.end())
        {
            pErrorStr = found->second;
        }
        else
        {
            pErrorStr = rapidjson::GetParseError_En(static_cast<ParseErrorCode>(errorCode));
            if (!pErrorStr && !returnNullOnError)
            {
                pErrorStr = pUnknownError;
            }
        }
    }

    return pErrorStr;
}

const char* JsonError::GetString(ParseErrorCode parseErrorCode, bool returnNullOnError)
{
    return GetString(static_cast<Code>(parseErrorCode), returnNullOnError);
}

optional<string> JsonError::GetCodeStr(intptr_t code)
{
    optional<string> str;

    const char* pStr = GetString(static_cast<JsonError::Code>(code), true);
    if (pStr && *pStr)
    {
        str = string(" ") + pStr;
    }

    return str;
}

///////////////////////////////////////////////////////////////////////////

XceptionJson::XceptionJson(JsonError::Code errorCode, const char* pMsg, const char* pStreamName)
    : XceptionFileT<JsonError::Code>(pStreamName, errorCode, pMsg, JsonError::GetCodeStr)
{
}

XceptionJson::XceptionJson(rapidjson::ParseErrorCode parseErrorCode, const char* pMsg, const char* pStreamName)
    : XceptionJson(static_cast<JsonError::Code>(parseErrorCode), pMsg, pStreamName)
{
}

XceptionJson::XceptionJson(intptr_t errorCode, const char* pMsg, const char* pStreamName)
    : XceptionJson(static_cast<JsonError::Code>(errorCode), pMsg, pStreamName)
{
}

XceptionJson::XceptionJson(const fs::path& path, JsonError::Code errorCode, const char* pMsg)
    : XceptionFileT<JsonError::Code>(path, errorCode, pMsg, JsonError::GetCodeStr)
{
}

XceptionJson::XceptionJson(const fs::path& path, rapidjson::ParseErrorCode parseErrorCode, const char* pMsg)
    : XceptionJson(path, static_cast<JsonError::Code>(parseErrorCode), pMsg)
{
}

XceptionJson::XceptionJson(const fs::path& path, intptr_t errorCode, const char* pMsg)
    : XceptionJson(path, static_cast<JsonError::Code>(errorCode), pMsg)
{
}

shared_ptr<fs::path> XceptionJson::getStreamName() const noexcept
{
    return XceptionFileT<JsonError::Code>::path();
}
