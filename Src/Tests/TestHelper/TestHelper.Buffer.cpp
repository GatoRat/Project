// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "TestHelper.Buffer.h"

using namespace std;

namespace TestHelper
{
    char* FillBuffer(char* pBuffer, size_t fillLen, char startChar, char endChar, bool terminate)
    {
        if (pBuffer && fillLen > 0)
        {
            if (terminate)
            {
                fillLen--;
            }

            char curChar;
            if (startChar > endChar)
            {
                curChar = endChar;
                endChar = startChar;
                startChar = curChar;
            }
            else
            {
                curChar = startChar;
            }

            size_t curOffset = 0;

            while (curOffset < fillLen)
            {
                pBuffer[curOffset++] = curChar;
                if (curChar++ == endChar)
                {
                    curChar = startChar;
                }
            }

            if (terminate)
            {
                pBuffer[curOffset] = 0;
            }
        }
        return pBuffer;
    }

    bool CheckBuffer(const char* pBuffer, size_t checkLen, char startChar, char endChar, bool terminate)
    {
        if (!pBuffer || !checkLen)
        {
            return false;
        }

        char curChar;
        if (startChar > endChar)
        {
            curChar = endChar;
            endChar = startChar;
            startChar = curChar;
        }
        else
        {
            curChar = startChar;
        }

        // curOffset is used this way to assist in debugging; place a breakpoint on
        // the "return false" line and curOffset will be at the point of failure.

        size_t curOffset = 0;
        const size_t endOffset = terminate ? checkLen - 1 : checkLen;

        for (; curOffset < endOffset; ++curOffset)
        {
            if (pBuffer[curOffset] != curChar)
            {
                break;
            }

            if (curChar++ == endChar)
            {
                curChar = startChar;
            }
        }

        if (terminate && curOffset == endOffset && pBuffer[curOffset] == 0)
        {
            ++curOffset;
        }

        if (curOffset != checkLen)
        {
            return false;
        }

        return true;
    }
}
