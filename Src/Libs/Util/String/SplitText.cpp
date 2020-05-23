// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "SplitText.h"

using namespace std;

void SplitText(string_view text, string& line1, string& line2)
{
    line1.resize(0);
    line2.resize(0);

    const size_t center      = text.length() / 2;
    const size_t offsetRight = text.find (' ', center);
    const size_t offsetLeft  = text.rfind(' ', center);

    if (offsetLeft == string::npos &&
        offsetRight == string::npos)
    {
        line1 = text;
    }
    else
    {
        size_t offset;

        if (offsetLeft == string::npos)
        {
            offset = offsetRight;
        }
        else if (offsetRight == string::npos)
        {
            offset = offsetLeft;
        }
        else
        {
            offset = center - offsetLeft < offsetRight - center ? offsetLeft : offsetRight;
        }

        line1 = text.substr(0, offset);
        line2 = text.substr(offset + 1);

        if (line1.empty())
        {
            line1 = move(line2);
        }
    }
}

void SplitText(string_view text, string_view& line1, string_view& line2)
{
    const size_t center      = text.length() / 2;
    const size_t offsetRight = text.find (' ', center);
    const size_t offsetLeft  = text.rfind(' ', center);

    if (offsetLeft  == string::npos &&
        offsetRight == string::npos)
    {
        line1 = text;
    }
    else
    {
        size_t offset;

        if (offsetLeft == string::npos)
        {
            offset = offsetRight;
        }
        else if (offsetRight == string::npos)
        {
            offset = offsetLeft;
        }
        else
        {
            offset = center - offsetLeft < offsetRight - center ? offsetLeft : offsetRight;
        }

        line1 = text.substr(0, offset);
        line2 = text.substr(offset + 1);

        if (line1.empty())
        {
            line1 = move(line2);
        }
    }
}
