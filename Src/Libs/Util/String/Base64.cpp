// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Base64.h"
#include "ASCII.h"
#include "../Math/RoundUp.h"

using namespace std;

static char Base64EncodeAlphabet[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static uint8_t Base64DecodeAlphabet[256] = 
{
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
//                                                           +                   /
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
//    0    1    2    3    4    5    6    7    8    9                   =
	 52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255, 255,  64, 255, 255,
//         A    B    C    D    E    F    G    H    I    J    K    L    M    N    O
	255,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
//    P    Q    R    S    T    U    V    W    X    Y    Z
	 15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
//         a    b    c    d    e    f    g    h    i    j    k    l    m    n    o
	255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
//    p    q    r    s    t    u    v    w    x    y    z
	 41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

string Base64Encode(const void* pSrc_, size_t srcLen, size_t lineLen, bool useOnlyLF)
{
    string encoded;

    if (srcLen)
    {
        encoded.reserve(RoundUp(srcLen * 3, 2) + (lineLen ? (srcLen / lineLen) * 2 : 0)); //-V107 //-V104

        const uint8_t* pSrc{ static_cast<const uint8_t*>(pSrc_) };

        size_t curLineLen = 0;

        for (size_t i = 0; i < srcLen; i += 3)
        {
            uint8_t ch[3];

            ch[0] = static_cast<uint8_t>(pSrc[i]);
            ch[1] = static_cast<uint8_t>((i + 1) < srcLen ? pSrc[i + 1] : 0);
            ch[2] = static_cast<uint8_t>((i + 2) < srcLen ? pSrc[i + 2] : 0);

            encoded.push_back(Base64EncodeAlphabet[ch[0] >> 2]);
            encoded.push_back(Base64EncodeAlphabet[((ch[0] & 0x03) << 4) | (ch[1] >> 4)]);

            if ((i + 1) < srcLen)
            {
                encoded.push_back(Base64EncodeAlphabet[((ch[1] & 0x0f) << 2) | (ch[2] >> 6)]);
            }
            else
            {
                encoded.push_back('=');
            }

            if ((i + 2) < srcLen)
            {
                encoded.push_back(Base64EncodeAlphabet[ch[2] & 0x3f]);
            }
            else
            {
                encoded.push_back('=');
            }

            curLineLen += 4; //-V112

            if (curLineLen >= lineLen)
            {
                if (!useOnlyLF)
                {
                    encoded.push_back(ASCII::CR);
                }
                encoded.push_back(ASCII::LF);
                curLineLen = 0;
            }
        }
    }

    return encoded;
}

vector<uint8_t> Base64Decode(const char* pSrc)
{
    vector<uint8_t> decoded;

    if (pSrc)
    {
        while (*pSrc)
        {
            uint8_t buffer[4]; //-V112

            do
            {
                buffer[0] = Base64DecodeAlphabet[static_cast<uint8_t>(*pSrc++)];
                if (buffer[0] != 255)
                {
                    break;
                }

            } while (*pSrc);

            if (buffer[0] == 255)
            {
                buffer[0] = 0;
            }

            if (*pSrc)
            {
                do
                {
                    buffer[1] = Base64DecodeAlphabet[static_cast<uint8_t>(*pSrc++)];
                    if (buffer[1] != 255)
                    {
                        break;
                    }

                } while (*pSrc);

                if (buffer[1] == 255)
                {
                    buffer[1] = 0;
                }
            }
            else
            {
                buffer[1] = 0;
            }

            if (*pSrc)
            {
                do
                {
                    buffer[2] = Base64DecodeAlphabet[static_cast<uint8_t>(*pSrc++)];
                    if (buffer[2] != 255)
                    {
                        break;
                    }

                } while (*pSrc);

                if (buffer[2] == 255)
                {
                    buffer[2] = 0;
                }
            }
            else
            {
                buffer[2] = 0;
            }

            if (*pSrc)
            {
                do
                {
                    buffer[3] = Base64DecodeAlphabet[static_cast<uint8_t>(*pSrc++)];
                    if (buffer[3] != 255)
                    {
                        break;
                    }

                } while (*pSrc);

                if (buffer[3] == 255)
                {
                    buffer[3] = 0;
                }
            }
            else
            {
                buffer[3] = 0;
            }

            decoded.push_back(static_cast<uint8_t>((buffer[0] << 2) | (buffer[1] >> 4)));

            if (buffer[2] != 64)
            {
                decoded.push_back(static_cast<uint8_t>((buffer[1] & 0x0f) << 4) | (buffer[2] >> 2));
                if (buffer[3] != 64)
                {
                    decoded.push_back(static_cast<uint8_t>(((buffer[2] & 0x03) << 6) | buffer[3]));
                }
            }
        }
    }

	return decoded;
}
