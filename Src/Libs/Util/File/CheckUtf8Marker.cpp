// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CheckUtf8Marker.h"
#include <string>
#include <istream>

using namespace std;

static int bom[]{ 0xEF, 0xBB, 0xBF };

bool CheckUtf8Marker(istream& stream, string& str)
{
    bool isUtf8 = false;

    for (int ch : bom)
    {
        if (stream.eof() || stream.peek() != ch)
        {
            break;
        }

        str.push_back(static_cast<char>(stream.get()));
    }

    if (str.size() == _countof(bom))
    {
        isUtf8 = true;
        str.resize(0);
    }

    return isUtf8;
}

static bool IsUtf8Marker(const char* pStr)
{
    return static_cast<uint8_t>(pStr[0]) == bom[0] &&
           static_cast<uint8_t>(pStr[1]) == bom[1] &&
           static_cast<uint8_t>(pStr[2]) == bom[2];
}

bool CheckUtf8Marker(const char*& pStr)
{
    const bool isUtf8 = IsUtf8Marker(pStr);
    if (isUtf8)
    {
        pStr += 3;
    }
    return isUtf8;
}

bool CheckUtf8Marker(char*& pStr)
{
    const bool isUtf8 = IsUtf8Marker(pStr);
    if (isUtf8)
    {
        pStr += 3;
    }
    return isUtf8;
}
