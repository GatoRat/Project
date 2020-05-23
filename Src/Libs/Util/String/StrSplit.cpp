// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "StrSplit.h"
#include <cstring>

using namespace std;

namespace Str
{
    vector<string_view> Split(string_view str, char delimiter)
    {
        vector<string_view> strings;

        Split(str, delimiter, [&](size_t /*num*/, const string_view& str)->bool
        {
            strings.push_back(str);
            return true;
        });

        return strings;
    }

    size_t Split(string_view str, char delimiter, const SplitCallback& callback)
    {
        size_t num = 0;

        for (size_t offset = 0;;)
        {
            size_t pos = str.find(delimiter, offset);
            if (!callback(num++, str.substr(offset, pos - offset)))
            {
                break;
            }

            if (pos == string_view::npos)
            {
                break;
            }

            offset = pos + 1;
            if (offset == str.size())
            {
                callback(num++, str.substr(offset, 0));
                break;
            }
        }

        return num;
    }

    size_t Split2(char* pStr, char delimiter, const SplitCallback2& callback)
    {
        size_t num = 0;

        while (pStr)
        {
            char* pEnd = strchr(pStr, static_cast<int>(delimiter));
            if (pEnd)
            {
                *pEnd++ = 0;
            }

            if (!callback(num++, pStr))
            {
                break;
            }

            pStr = pEnd;
        }

        return num;
    }

} // namespace Str
