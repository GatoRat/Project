// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <optional>
#include <string>
#include <vector>
#include <RapidJSON/rapidjson.h>
#include <RapidJSON/writer.h>
#include "../Math/Almost.h"
#include "../String/Convert.h"
#include "../String/Str.h"
#include "../Time/TimeStamp.h"

#ifndef OUR_RAPIDJSON_MACROS
    #error rapidjson.h must have the following block defined at its top.
        //#ifndef RAPIDJSON_ASSERT
        //    #define RAPIDJSON_ASSERT
        //#endif
        //#ifndef RAPIDJSON_HAS_STDSTRING
        //    #define RAPIDJSON_HAS_STDSTRING 1
        //#endif
        //#define OUR_RAPIDJSON_MACROS
#endif

struct JsonWriter : public rapidjson::Writer<rapidjson::StringBuffer>
{
    rapidjson::StringBuffer buffer;
protected:

public:

    explicit JsonWriter(bool startObject = true)
        : rapidjson::Writer<rapidjson::StringBuffer>(buffer) //-V1050
    {
        if (startObject)
        {
            StartObject();
        }
    }

    std::string getString()
    {
        if (!IsComplete())
        {
            EndObject();
        }
        return buffer.GetString();
    }

    struct Object final
    {
        JsonWriter& writer;

        Object(const char* pKey, JsonWriter& jsonWriter)
            : writer{ jsonWriter }
        {
            if (pKey && *pKey)
            {
                writer.Key(pKey);
            }
            writer.StartObject();
        }

        explicit Object(JsonWriter& jsonWriter)
            : writer{ jsonWriter }
        {
            writer.StartObject();
        }

        ~Object()
        {
            writer.EndObject();
        }
    };

    struct Array final
    {
        JsonWriter& writer;

        Array(const char* pKey, JsonWriter& jsonWriter)
            : writer{ jsonWriter }
        {
            writer.Key(pKey);
            writer.StartArray();
        }

        ~Array()
        {
            writer.EndArray();
        }
    };

    void writer(JsonWriter& jsonWriter)
    {
        auto str = jsonWriter.getString();
        RawValue(str.data(), str.size(), rapidjson::kObjectType);
    }

    void clear()
    {
        buffer.Clear();
    }

    void keyValChar(const char* pKey, char ch)
    {
        Key(pKey);
        String(&ch, 1);
    }

    void keyVal(const char* pKey, bool val)
    {
        Key(pKey);
        Bool(val);
    }

    void keyVal(const char* pKey, int val)
    {
        Key(pKey);
        Int(val);
    }

    void keyVal(const char* pKey, unsigned val)
    {
        Key(pKey);
        Uint(val);
    }

    void keyVal(const char* pKey, int64_t val)
    {
        Key(pKey);
        Int64(val);
    }

    void keyVal(const char* pKey, uint64_t val)
    {
        Key(pKey);
        Uint64(val);
    }

    void keyVal(const char* pKey, double val)
    {
        Key(pKey);
        Double(val);
    }

    void keyVal(const char* pKey, float val)
    {
        Key(pKey);
        Double(val);
    }

    void keyVal(const char* pKey, const char* pStr, int len = -1)
    {
        Key(pKey);
        if (pStr)
        {
            String(pStr, static_cast<rapidjson::SizeType>(len < 0 ? strlen(pStr) : len)); //-V105
        }
        else
        {
            Null();
        }
    }

    void keyVal(const char* pKey, const std::string& str)
    {
        Key(pKey);
        String(str);
    }

    void keyVal(const char* pKey, const std::string_view& str)
    {
        Key(pKey);
        String(str.data(), static_cast<rapidjson::SizeType>(str.size()));
    }

    void keyVal(const char* pKey, const wchar_t* pStr, int len = -1)
    {
        if (pStr)
        {
            keyVal(pKey, Convert::ToUtf8(pStr, len));
        }
        else
        {
            keyNull(pKey);
        }
    }

    void keyVal(const char* pKey, const std::wstring& str)
    {
        keyVal(pKey, Convert::ToUtf8(str));
    }

    void keyVal(const char* pKey, const std::wstring_view& str)
    {
        keyVal(pKey, Convert::ToUtf8(str));
    }

#if __has_include(<qstring.h>)

    void qKeyVal(const char* pKey, const QByteArray& str)
    {
        keyVal(pKey, str.data(), str.size());
    }

    void qKeyVal(const char* pKey, QStringView newStr) //-V813
    {
        qKeyVal(pKey, newStr.toUtf8());
    }

#endif

    void keyVal(const char* pKey, TimeStamp val)
    {
        keyVal(pKey, val.val);
    }

    void keyNull(const char* pKey)
    {
        Key(pKey);
        Null();
    }

    ///////////////////////////////////////////////////////////////////////

    void optionalKeyVal(const char* pKey, bool val, bool defaultVal)
    {
        if (val != defaultVal)
        {
            Key(pKey);
            Bool(val);
        }
    }

    void optionalKeyVal(const char* pKey, int val, int defaultVal)
    {
        if (val != defaultVal)
        {
            keyVal(pKey, val);
        }
    }

    void optionalKeyVal(const char* pKey, unsigned val, unsigned defaultVal)
    {
        if (val != defaultVal)
        {
            keyVal(pKey, val);
        }
    }

    void optionalKeyVal(const char* pKey, int64_t val, int64_t defaultVal)
    {
        if (val != defaultVal)
        {
            keyVal(pKey, val);
        }
    }

    void optionalKeyVal(const char* pKey, uint64_t val, uint64_t defaultVal)
    {
        if (val != defaultVal)
        {
            keyVal(pKey, val);
        }
    }

    void optionalKeyVal(const char* pKey, float val, float defaultVal)
    {
        if (!Almost::Equal(val, defaultVal))
        {
            keyVal(pKey, val);
        }
    }

    void optionalKeyVal(const char* pKey, const char* pStr)
    {
        if (pStr)
        {
            keyVal(pKey, pStr);
        }
    }

    void optionalKeyVal(const char* pKey, const char* pStr, const char* pDefault)
    {
        if (pStr && pDefault && !Str::AreEqual(pStr, pDefault))
        {
            keyVal(pKey, pStr);
        }
    }

    void optionalKeyVal(const char* pKey, const std::string& str)
    {
        if (!str.empty())
        {
            keyVal(pKey, str);
        }
    }

    void optionalKeyVal(const char* pKey, const std::string& str, const char* pDefault)
    {
        if (pDefault ? str != pDefault : !str.empty())
        {
            keyVal(pKey, str);
        }
    }

    void optionalKeyVal(const char* pKey, const std::string_view& str)
    {
        if (!str.empty())
        {
            keyVal(pKey, str);
        }
    }

    void optionalKeyVal(const char* pKey, const std::string_view& str, const char* pDefault)
    {
        if (pDefault ? str != pDefault : !str.empty())
        {
            keyVal(pKey, str);
        }
    }

    void optionalKeyVal(const char* pKey, std::wstring_view str, const wchar_t* pDefault)
    {
        if (pDefault ? str != pDefault : !str.empty())
        {
            keyVal(pKey, str);
        }
    }

    ///////////////////////////////////////////////////////////////////////

    void optionalKeyVal(const char* pKey, const std::optional<bool>& val)
    {
        if (val)
        {
            keyVal(pKey, val.value());
        }
    }

    void optionalKeyVal(const char* pKey, const std::optional<int>& val)
    {
        if (val)
        {
            keyVal(pKey, val.value());
        }
    }

    void optionalKeyVal(const char* pKey, const std::optional<unsigned>& val)
    {
        if (val)
        {
            keyVal(pKey, val.value());
        }
    }

    void optionalKeyVal(const char* pKey, const std::optional<int64_t>& val)
    {
        if (val)
        {
            keyVal(pKey, val.value());
        }
    }

    void optionalKeyVal(const char* pKey, const std::optional<uint64_t>& val)
    {
        if (val)
        {
            keyVal(pKey, val.value());
        }
    }

    void optionalKeyVal(const char* pKey, const std::optional<float>& val)
    {
        if (val)
        {
            keyVal(pKey, val.value());
        }
    }

    void optionalKeyVal(const char* pKey, const std::optional<std::string>& str)
    {
        if (str && !str.value().empty())
        {
            keyVal(pKey, str.value());
        }
    }

    void optionalKeyVal(const char* pKey, const std::optional<std::wstring>& str)
    {
        if (str && !str.value().empty())
        {
            keyVal(pKey, str.value());
        }
    }

    void optionalKeyVal(const char* pKey, const std::optional<TimeStamp>& val)
    {
        if (val)
        {
            keyVal(pKey, val->val);
        }
    }

    ///////////////////////////////////////////////////////////////////////

    void optionalKeyVal(const char* pKey, const std::optional<bool>& val, bool defaultVal)
    {
        if (val && val != defaultVal)
        {
            Key(pKey);
            Bool(val.value());
        }
    }

    void optionalKeyVal(const char* pKey, const std::optional<int>& val, int defaultVal)
    {
        if (val && val != defaultVal)
        {
            keyVal(pKey, val.value());
        }
    }

    void optionalKeyVal(const char* pKey, const std::optional<unsigned>& val, unsigned defaultVal)
    {
        if (val && val != defaultVal)
        {
            keyVal(pKey, val.value());
        }
    }

    void optionalKeyVal(const char* pKey, const std::optional<int64_t>& val, int64_t defaultVal)
    {
        if (val && val != defaultVal)
        {
            keyVal(pKey, val.value());
        }
    }

    void optionalKeyVal(const char* pKey, const std::optional<uint64_t>& val, uint64_t defaultVal)
    {
        if (val && val != defaultVal)
        {
            keyVal(pKey, val.value());
        }
    }

    void optionalKeyVal(const char* pKey, const std::optional<float>& val, float defaultVal)
    {
        if (val && !Almost::Equal(val.value(), defaultVal))
        {
            keyVal(pKey, val.value());
        }
    }

    ///////////////////////////////////////////////////////////////////////

#if __has_include(<qstring.h>)

    bool ifOptionalKeyVal(const char* pKey, const std::optional<QString>& str, QStringView newStr) //-V813
    {
        bool changed = false;
        /*
            If str has a value:
                - compare it to newStr
                - If same, don't write anything
                - If different
                    - If newStr is empty, write null
                    - else write string
            If str doesn't have a value
                - If newStr is NOT empty, write string

        */
        if (str)
        {
            if (str != newStr)
            {
                if (newStr.isEmpty())
                {
                    keyNull(pKey);
                }
                else
                {
                    qKeyVal(pKey, newStr);
                }
                changed = true;
            }
        }
        else if (!newStr.isEmpty())
        {
            qKeyVal(pKey, newStr);
            changed = true;
        }

        return changed;
    }

#endif //__has_include(<qstring.h>)

    bool ifOptionalKeyVal(const char* pKey, const std::optional<std::string>& str, std::string_view newStr)
    {
        bool changed = false;
        /*
            If str has a value:
                - compare it to newStr
                - If same, don't write anything
                - If different
                    - If newStr is empty, write null
                    - else write string
            If str doesn't have a value
                - If newStr is NOT empty, write string
        
        */
        if (str)
        {
            if (str != newStr)
            {
                if (newStr.empty())
                {
                    keyNull(pKey);
                }
                else
                {
                    keyVal(pKey, newStr);
                }
                changed = true;
            }
        }
        else if (!newStr.empty())
        {
            keyVal(pKey, newStr);
            changed = true;
        }

        return changed;
    }

    bool ifOptionalKeyVal(const char* pKey, const std::optional<int64_t>& val, int64_t newVal, bool nullOnZero = true)
    {
        bool changed = false;
        /*
            If val has a value:
                - compare it to newVal
                - If same, don't write anything
                - If different
                    - If newVal is zero and nullOnZero is true, write null
                    - else write newVal
            If str doesn't have a value
                - If newStr is NOT empty, write string

        */
        if (val)
        {
            if (val != newVal)
            {
                if (newVal == 0 && nullOnZero)
                {
                    keyNull(pKey);
                }
                else
                {
                    keyVal(pKey, newVal);
                }
                changed = true;
            }
        }
        else if (newVal || !nullOnZero)
        {
            keyVal(pKey, newVal);
            changed = true;
        }

        return changed;
    }

    ///////////////////////////////////////////////////////////////////////

    void array(const uint8_t* pData, size_t dataLen)
    {
        if (pData)
        {
            for (size_t i = 0; i < dataLen; ++i)
            {
                Uint(pData[i]);
            }
        }
    }

    void keyVal(const char* pName, const uint8_t* pData, size_t dataLen)
    {
        Array jArray(pName, *this);

        array(pData, dataLen);
    }

    void keyVal(const char* pName, const std::vector<uint8_t>& data)
    {
        Array jArray(pName, *this);

        for (auto val : data)
        {
            Uint(val);
        }
    }

    void keyVal(const char* pName, const std::optional<std::vector<uint8_t>>& data)
    {
        if (data && !data.value().empty())
        {
            Array jArray(pName, *this);

            for (auto val : data.value())
            {
                Uint(val);
            }
        }
    }
};
