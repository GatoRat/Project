// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "JsonPath.h"
#include <stdexcept>
#include "Util/String/Str.h"

using namespace std;

JsonPath::JsonPath(string_view jsonPath)
{
    _tokens.reserve(4); //-V112
    append(jsonPath);
}

JsonPath::JsonPath(string_view jsonPath1, string_view jsonPath2)
{
    _tokens.reserve(4); //-V112
    append(jsonPath1);
    append(jsonPath2);
}

JsonPath::JsonPath(const JsonPath& jsonPath1, string_view jsonPath2)
{
    _tokens.reserve(4); //-V112
    append(jsonPath1);
    append(jsonPath2);
}

string_view JsonPath::operator[](size_t index) const //-V2506
{
    try
    {
        return _tokens.at(index);
    }
    catch (out_of_range&)
    {
        static string_view emptyView;
        return emptyView;
    }
}

char JsonPath::front() const
{
    return isEmpty() ? 0 : _tokens[0].front();
}

string JsonPath::toPointer(const char* pName) const
{
    string str;

    for (auto& token : _tokens)
    {
        str += '/';
        str += token;
    }

    if (pName)
    {
        if (*pName != '/')
        {
            str += '/';
        }
        str += pName;
    }

    return str;
}

JsonPath::Filename JsonPath::toFilename() const
{
    Filename filename;

    if (!_tokens.empty())
    {
        size_t numTokens = _tokens.size();

        {
            auto& token = _tokens.back();

            static const char* exceptions[]{ "file", "files", "font", "fonts", "sheet", "sheets" };
            for (const char* pException : exceptions)
            {
                if (token == pException)
                {
                    filename.isPlural = token.back() == 's';
                    --numTokens;
                    break;
                }
            }
        }

        for (size_t token = 0; token < numTokens; ++token)
        {
            if (token == 1 && (Str::Compare(_tokens[0].c_str(), "Fonts", true) == 0 ||
                               Str::Compare(_tokens[0].c_str(), "Sounds", true) == 0))
            {
                filename.filename += '/';
            }
            else if (token)
            {
                filename.filename += '.';
            }
            filename.filename += _tokens[token];
        }
    }

    return filename;
}

JsonPath::Filename JsonPath::toFilename(const char* pPath) const
{
    Filename filename;

    if (Str::IsEmptyOrWhiteSpace(pPath))
    {
        filename = toFilename();
    }
    else
    {
        filename = JsonPath(*this, pPath).toFilename();
    }

    return filename;
}

size_t JsonPath::find(string_view tokenToFind) const
{
    size_t index = npos;
    if (!tokenToFind.empty())
    {
        for (size_t i = 0; i < _tokens.size(); ++i)
        {
            if (tokenToFind == _tokens[i])
            {
                index = i;
                break;
            }
        }
    }
    return index;
}

JsonPath& JsonPath::append(string_view jsonPath)
{
    do
    {
        while (!jsonPath.empty() && jsonPath.front() == '/')
        {
            jsonPath.remove_prefix(1);
        }

        if (!jsonPath.empty())
        {
            const size_t pos = jsonPath.find('/');
            if (pos == string_view::npos)
            {
                _tokens.emplace_back(jsonPath);
                jsonPath.remove_suffix(jsonPath.size());
            }
            else
            {
                _tokens.emplace_back(jsonPath.substr(0, pos));
                jsonPath.remove_prefix(pos + 1);
            }
        }

    } while (!jsonPath.empty());

    return *this;
}

JsonPath& JsonPath::append(const JsonPath& jsonPath)
{
    _tokens.insert(_tokens.end(), jsonPath._tokens.begin(), jsonPath._tokens.end());
    return *this;
}

JsonPath& JsonPath::replaceEnd(string_view jsonPath)
{
    removeEnd();
    return append(jsonPath);
}

JsonPath& JsonPath::replaceEnd(const JsonPath& jsonPath)
{
    removeEnd();
    return append(jsonPath);
}

JsonPath& JsonPath::removeEnd()
{
    if (!_tokens.empty())
    {
        _tokens.pop_back();
    }
    return *this;
}

JsonPath& JsonPath::removeAfter(size_t index)
{
    if (index < _tokens.size() - 1)
    {
        _tokens.resize(index + 1);
    }

    return *this;
}

JsonPath& JsonPath::removeAfter(string_view token)
{
    return removeAfter(find(token));
}

string_view JsonPath::GetLastToken(string_view jsonPath)
{
    while (!jsonPath.empty() && jsonPath.back() == '/')
    {
        jsonPath.remove_suffix(1);
    }

    if (!jsonPath.empty())
    {
        const size_t pos = jsonPath.rfind('/');
        if (pos != string_view::npos)
        {
            jsonPath.remove_prefix(pos + 1);
        }
    }

    return jsonPath;
}

JsonPath::Result JsonPath::search(const rapidjson::Value* pValue, const char* pName, bool allowRoot) const
{
    JsonPath::Result result;

    if (pValue)
    {
        JsonPath jsonPath(pName ? pName : "");

        if (jsonPath.isEmpty())
        {
            result = get(pValue);
        }
        else if (isEmpty())
        {
            result = jsonPath.get(pValue);
        }
        else
        {
            vector<const rapidjson::Value*> values;

            result = get(pValue, &values);
            if (!result.haltSearch)
            {
                result.reset();

                while (values.size() > (allowRoot ? 0 : 1))
                {
                    pValue = values.back();
                    values.pop_back();

                    result = jsonPath.get(pValue, nullptr);
                    if (result.haltSearch || result.pValue)
                    {
                        break;
                    }
                }
            }
        }
    }

    return result;
}

JsonPath::Result JsonPath::get(const rapidjson::Value* pValue) const
{
    return get(pValue, nullptr);
}

JsonPath::Result JsonPath::get(const rapidjson::Value* pValue, vector<const rapidjson::Value*>* pValues) const
{
    JsonPath::Result result;

    if (pValue && !_tokens.empty())
    {
        for (auto tokenIt = _tokens.begin();;)
        {
            if (pValue->GetType() == rapidjson::kObjectType)
            {
                if (pValue->IsNull())
                {
                    result.haltSearch = true;
                    break;
                }

                if (pValues)
                {
                    pValues->push_back(pValue);
                }

                auto memberIt = pValue->FindMember(*tokenIt);
                if (memberIt == pValue->MemberEnd())
                {
                    break;
                }

                pValue = &memberIt->value;
                if (++tokenIt == _tokens.end())
                {
                    if (pValue->IsNull())
                    {
                        result.haltSearch = true;
                    }
                    else
                    {
                        if (pValues)
                        {
                            pValues->push_back(pValue);
                        }
                        result.pValue = pValue;
                    }
                    break;
                }
            }
            else
            {
                if (pValue->GetType() == rapidjson::kArrayType)
                {
                    if (tokenIt == _tokens.end())
                    {
                        result.pValue = pValue;
                        break;
                    }
                }

                result.haltSearch = true;
                break;
            }
        }
    }
    else
    {
        result.haltSearch = true;
    }

    return result;
}
