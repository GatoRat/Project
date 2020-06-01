// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "SecureContainer.h"
#include <lz4/lz4.h>
#include <lz4/lz4hc.h>
#include "../Error/Xception.h"
#include "../File/FastFile.h"
#include "../File/XceptionFile.h"
#include "../Math/Random.h"
#include "../String/ASCII.h"
#include "AES_GCM.h"

using namespace std;

#pragma warning(error : 4061) // enumerator 'identifier' in switch of enum 'enumeration' is not explicitly handled by a case label
#pragma warning(error : 4062) // enumerator 'identifier' in switch of enum 'enumeration' is not handled

namespace SecureContainer
{
    uint8_t CopyCompressor::getType() const
    {
        return NoCompression;
    }

    int32_t CopyCompressor::calcMaxOutputSize(int32_t dataLen) const
    {
        return dataLen;
    }

    int32_t CopyCompressor::compress(const void* pSrc, int32_t srcLen, void* pDst, int32_t maxOutputSize) const
    {
        return Decompress(pSrc, srcLen, pDst, maxOutputSize);
    }

    int32_t CopyCompressor::Decompress(const void* pSrc, int32_t srcLen, void* pDst, int32_t dstLen)
    {
        const int32_t copyLen = min(srcLen, dstLen);
        memcpy(pDst, pSrc, copyLen); //-V106
        return copyLen;
    }

    ///////////////////////////////////////////////////////////////////////////

    LZ4Compressor::LZ4Compressor(int acceleration_)
        : acceleration(max(DefaultAcceleration, acceleration_))
    {
    }

    uint8_t LZ4Compressor::getType() const
    {
        return LZ4Compressed;
    }

    int32_t LZ4Compressor::calcMaxOutputSize(int32_t dataLen) const
    {
        return LZ4_compressBound(dataLen);
    }

    int32_t LZ4Compressor::compress(const void* pSrc, int32_t srcLen, void* pDst, int32_t maxOutputSize) const
    {
        return LZ4_compress_fast(static_cast<const char*>(pSrc), reinterpret_cast<char*>(pDst), srcLen, maxOutputSize, acceleration);
    }

    int32_t LZ4Compressor::Decompress(const void* pSrc, int32_t srcLen, void* pDst, int32_t dstLen)
    {
        return LZ4_decompress_safe(static_cast<const char*>(pSrc), reinterpret_cast<char*>(pDst), srcLen, dstLen);
    }

    ///////////////////////////////////////////////////////////////////////////

    LZ4HighCompressor::LZ4HighCompressor(int level_)
        : LZ4Compressor(max(MinLevel, level_))
    {
    }

    int32_t LZ4HighCompressor::compress(const void* pSrc, int32_t srcLen, void* pDst, int32_t maxOutputSize) const
    {
        return LZ4_compress_HC(static_cast<const char*>(pSrc), reinterpret_cast<char*>(pDst), srcLen, maxOutputSize, level);
    }

    ///////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

    struct IvTag
    {
        uint8_t iv[AES_GCM::IvLen];
        uint8_t tag[AES_GCM::TagLen];
    };

#pragma pack(pop)

    void Encode(
        const void*         pData,
        int32_t             dataLen,
        vector<uint8_t>&    encodedData,
        const uint8_t*      pKey,
        const  ICompressor* pCompressor,
        int32_t             minCompressLen)
    {
        encodedData.resize(0);

        if (pData && dataLen > 0 && pKey)
        {
            if (!pCompressor || dataLen < minCompressLen)
            {
                static CopyCompressor copyCompressor;
                pCompressor = &copyCompressor;
            }

            const int32_t compressedCapacity = pCompressor->calcMaxOutputSize(dataLen); //-V522

            // Resize to worse case
            encodedData.resize(sizeof(uint8_t) + sizeof(int32_t) + sizeof(IvTag) + compressedCapacity); //-V119 //-V104

            int32_t encryptLen = 1;
            uint8_t* pDst = encodedData.data();
            *pDst = pCompressor->getType();

            if (*pDst)
            {
                *reinterpret_cast<int32_t*>(&pDst[1]) = dataLen; //-V206
                encryptLen += sizeof(int32_t);
            }

            const int32_t compressedLen = pCompressor->compress(pData, dataLen, &pDst[encryptLen], compressedCapacity);
            if (compressedLen <= 0)
            {
                throw Xception("Error: Compression failed.");
            }

            encryptLen += compressedLen;

            IvTag* pIvTag = reinterpret_cast<IvTag*>(&pDst[encryptLen]); //-V108

            RandBuffer(pIvTag->iv, sizeof(pIvTag->iv));

            const int result = AES_GCM::encrypt(pKey, pIvTag->iv, nullptr, pDst, encryptLen, pIvTag->tag); //-V106
            if (result != encryptLen)
            {
                throw Xception("Error: Encryption failed.");
            }

            encodedData.resize(encryptLen + sizeof(IvTag)); //-V119 //-V104
        }
    }

    void Encode(
        const string&      str,
        vector<uint8_t>&   encodedData,
        const uint8_t*     pKey,
        const ICompressor* pCompressor,
        int32_t            minCompressLen)
    {
        return Encode(str.c_str(), static_cast<int32_t>(str.size() + 1), encodedData, pKey, pCompressor, minCompressLen);
    }

    vector<uint8_t> Encode(
        const void*        pData,
        int32_t            dataLen,
        const uint8_t*     pKey,
        const ICompressor* pCompressor,
        int32_t            minCompressLen)
    {
        vector<uint8_t> data;
        Encode(pData, dataLen, data, pKey, pCompressor, minCompressLen);
        return data;
    }

    void Encode(
        const fs::path&    pathname,
        const void*        pData,
        int32_t            dataLen,
        const uint8_t*     pKey,
        const ICompressor* pCompressor,
        int32_t            minCompressLen)
    {
        vector<uint8_t> encodedData = Encode(pData, dataLen, pKey, pCompressor, minCompressLen);
        FastWriteFile(pathname, encodedData.data(), static_cast<uint32_t>(encodedData.size()));
    }

    vector<uint8_t> Encode(
        const string&      str,
        const uint8_t*     pKey,
        const ICompressor* pCompressor,
        int32_t            minCompressLen)
    {
        return Encode(str.c_str(), static_cast<int32_t>(str.size() + 1), pKey, pCompressor, minCompressLen);
    }

    void Encode(
        const fs::path&    pathname,
        const string&      str,
        const uint8_t*     pKey,
        const ICompressor* pCompressor,
        int32_t            minCompressLen)
    {
        return Encode(pathname, str.c_str(), static_cast<int32_t>(str.size() + 1), pKey, pCompressor, minCompressLen);
    }

    vector<uint8_t> Decode(const fs::path& pathname, int32_t maxLen, const uint8_t* pKey)
    {
        vector<uint8_t> data;

        if (maxLen >= 0)
        {
            int64_t fileSize = 0;
            data = FastReadFile(pathname, static_cast<uint32_t>(maxLen), &fileSize);

            if (fileSize > static_cast<int64_t>(maxLen))
            {
                throw XceptionFile(pathname, 0, "File is too large.");
            }

            if (fileSize < sizeof(IvTag))
            {
                throw XceptionFile(pathname, 0, "File is too small.");
            }

            if (!data.empty())
            {
                data = Decode(data.data(), data.size(), maxLen, pKey);
            }
        }

        return data;
    }

    vector<uint8_t> Decode(void* pContainer, size_t containerLen, int32_t maxLen, const uint8_t* pKey)
    {
        vector<uint8_t> contents;

        if (containerLen)
        {
            if (!pContainer || maxLen < 0 || containerLen > static_cast<size_t>(maxLen))
            {
                throw Xception(0, "Invalid or corrupt data.");
            }

            uint8_t* pSrc = static_cast<uint8_t*>(pContainer);
            int32_t  contentLen = static_cast<int32_t>(containerLen - sizeof(IvTag));
            IvTag*   pIvTag = reinterpret_cast<IvTag*>(&pSrc[containerLen] - sizeof(IvTag));

            const int result = AES_GCM::decrypt(pKey, pIvTag->iv, nullptr, pSrc, contentLen, pIvTag->tag); //-V106
            if (result != contentLen)
            {
                throw Xception(0, "Invalid or corrupt data.");
            }

            DecompressFunction decompressFunction;
            int32_t uncompressedLen = 0;

            --contentLen;
            uint8_t compressionType = *pSrc++;
            if (compressionType)
            {
                contentLen -= sizeof(int32_t);
                uncompressedLen = *reinterpret_cast<int32_t*>(pSrc); //-V206
                if (uncompressedLen > maxLen)
                {
                    throw Xception(0, "Invalid or corrupt data.");
                }

                pSrc += sizeof(int32_t);

                switch (compressionType)
                {
                    case LZ4Compressed:
                        decompressFunction = LZ4Compressor::Decompress;
                        break;

                    default:
                        throw Xception(0, "Invalid or corrupt data.");
                }
            }
            else
            {
                uncompressedLen = contentLen;
                decompressFunction = CopyCompressor::Decompress;
            }

            contents.resize(uncompressedLen); //-V106
            const int32_t decompressedLen = decompressFunction(pSrc, contentLen, contents.data(), uncompressedLen);
            if (decompressedLen != uncompressedLen)
            {
                throw Xception(0, "Invalid or corrupt data.");
            }
        }

        return contents;
    }

    void Decode(void* pContainer, size_t containerLen, int32_t maxLen, const uint8_t* pKey, const std::function<void*(int32_t finalLen)>& getFinalBuffer)
    {
        if (containerLen)
        {
            if (!pContainer || maxLen < 0 || containerLen > static_cast<size_t>(maxLen))
            {
                throw Xception(0, "Invalid or corrupt data.");
            }

            uint8_t* pSrc = static_cast<uint8_t*>(pContainer);
            int32_t  contentLen = static_cast<int32_t>(containerLen - sizeof(IvTag));
            IvTag*   pIvTag = reinterpret_cast<IvTag*>(&pSrc[containerLen] - sizeof(IvTag));

            const int result = AES_GCM::decrypt(pKey, pIvTag->iv, nullptr, pSrc, contentLen, pIvTag->tag); //-V106
            if (result != contentLen)
            {
                throw Xception(0, "Invalid or corrupt data.");
            }

            DecompressFunction decompressFunction;
            int32_t uncompressedLen = 0;

            --contentLen;
            uint8_t compressionType = *pSrc++;
            if (compressionType)
            {
                contentLen -= sizeof(int32_t);
                uncompressedLen = *reinterpret_cast<int32_t*>(pSrc); //-V206
                if (uncompressedLen > maxLen)
                {
                    throw Xception(0, "Invalid or corrupt data.");
                }

                pSrc += sizeof(int32_t);

                switch (compressionType)
                {
                    case LZ4Compressed:
                        decompressFunction = LZ4Compressor::Decompress;
                        break;

                    default:
                        throw Xception(0, "Invalid or corrupt data.");
                }
            }
            else
            {
                uncompressedLen = contentLen;
                decompressFunction = CopyCompressor::Decompress;
            }

            const int32_t decompressedLen = decompressFunction(pSrc, contentLen, getFinalBuffer(uncompressedLen), uncompressedLen);
            if (decompressedLen != uncompressedLen)
            {
                throw Xception(0, "Invalid or corrupt data.");
            }
        }
    }
}