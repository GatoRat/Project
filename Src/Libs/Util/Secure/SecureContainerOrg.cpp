// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "SecureContainerOrg.h"
#include <lz4/lz4.h>
#include <lz4/lz4hc.h>
#include "../Error/Xception.h"
#include "../File/FastFile.h"
#include "../File/XceptionFile.h"
#include "../Math/Random.h"
#include "../Misc/MakeFourCC.h"
#include "../String/ASCII.h"
#include "AES_GCM.h"

using namespace std;

#pragma warning(error : 4061) // enumerator 'identifier' in switch of enum 'enumeration' is not explicitly handled by a case label
#pragma warning(error : 4062) // enumerator 'identifier' in switch of enum 'enumeration' is not handled

namespace SecureContainerOrg
{
    CompressionType CopyCompressor::getType() const
    {
        return CompressionType::None;
    }

    uint32_t CopyCompressor::getFourCC() const
    {
        return 0;
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

    CompressionType LZ4Compressor::getType() const
    {
        return CompressionType::LZ4;
    }

    uint32_t LZ4Compressor::getFourCC() const
    {
        return MakeFourCC('L', 'Z', '4', 0);
        //return MakeFourCC('L', 'Z', 'M', 'A');
        //return MakeFourCC('Z', 'L', 'I', 'B');
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

    struct ContentHdr
    {
        uint32_t fourcc;
        uint32_t uncompressedLen;
    };

    struct IvTag
    {
        uint8_t iv[AES_GCM::IvLen];
        uint8_t tag[AES_GCM::TagLen];
    };

#pragma pack(pop)

    bool FileHdr::isSizeValid(size_t maxSize) const
    {
        const size_t actualContentLen = static_cast<size_t>(contentLen < 0 ? -contentLen : contentLen);
        return actualContentLen >= sizeof(ContentHdr) && (actualContentLen - sizeof(ContentHdr)) <= maxSize;
    }

    void Encode(
        const void*      pData,
        int32_t          dataLen,
        vector<uint8_t>& encodedData,
        uint32_t         fourcc,
        ICompressor*     pCompressor,
        const uint8_t*   pKey,
        const void*      pIV)
    {
        encodedData.resize(0);

        if (pData && dataLen)
        {
            if (!pCompressor)
            {
                if (!pKey && *reinterpret_cast<const char*>(pData) == ASCII::LeftBrace)
                {
                    encodedData.resize(dataLen); //-V106
                    memcpy(encodedData.data(), pData, dataLen); //-V106
                }
                else
                {
                    static CopyCompressor copyCompressor;
                    pCompressor = &copyCompressor;
                }
            }

            if (encodedData.empty())
            {
                const int32_t compressedCapacity = pCompressor->calcMaxOutputSize(dataLen); //-V522

                // Resize to worse case
                encodedData.resize(sizeof(FileHdr) + sizeof(ContentHdr) + (pKey ? sizeof(IvTag) : 0) + compressedCapacity); //-V119 //-V104

                FileHdr* pFileHdr = reinterpret_cast<FileHdr*>(encodedData.data());
                pFileHdr->fourcc  = fourcc;

                ContentHdr* pContentHdr      = reinterpret_cast<ContentHdr*>(&pFileHdr[1]);
                pContentHdr->fourcc          = pCompressor->getFourCC();
                pContentHdr->uncompressedLen = dataLen;

                const int32_t compressedLen = pCompressor->compress(pData, dataLen, &pContentHdr[1], compressedCapacity);
                if (compressedLen <= 0)
                {
                    throw Xception("Error: Compression failed.");
                }

                pFileHdr->contentLen = sizeof(ContentHdr) + compressedLen; //-V104 //-V103

                if (pKey)
                {
                    uint8_t* pContents = reinterpret_cast<uint8_t*>(pContentHdr);
                    IvTag* pIvTag = reinterpret_cast<IvTag*>(&pContents[pFileHdr->contentLen]); //-V108

                    if (pIV)
                    {
                        memcpy(pIvTag->iv, pIV, sizeof(pIvTag->iv));
                    }
                    else
                    {
                        RandBuffer(pIvTag->iv, sizeof(pIvTag->iv));
                    }

                    const int result = AES_GCM::encrypt(pKey, pIvTag->iv, nullptr, pContents, pFileHdr->contentLen, //-V106
                                                        pIvTag->tag, pFileHdr, sizeof(FileHdr));

                    if (result != pFileHdr->contentLen)
                    {
                        throw Xception("Error: Encryption failed.");
                    }

                    encodedData.resize(sizeof(FileHdr) + sizeof(IvTag) + pFileHdr->contentLen); //-V119 //-V104
                }
                else
                {
                    encodedData.resize(sizeof(FileHdr) + pFileHdr->contentLen); //-V104
                    pFileHdr->contentLen = -pFileHdr->contentLen;
                }
            }
        }
    }

    vector<uint8_t> Encode(
        const void*    pData,
        int32_t        dataLen,
        uint32_t       fourcc,
        ICompressor*   pCompressor,
        const uint8_t* pKey,
        const void*    pIV)
    {
        vector<uint8_t> data;
        Encode(pData, dataLen, data, fourcc, pCompressor, pKey, pIV);
        return data;
    }

    void Encode(
        const fs::path&  pathname,
        const void*      pData,
        int32_t          dataLen,
        uint32_t         fourcc,
        ICompressor*     pCompressor,
        const uint8_t*   pKey,
        const void*      pIV)
    {
        vector<uint8_t> encodedData = Encode(pData, dataLen, fourcc, pCompressor, pKey, pIV);
        FastWriteFile(pathname, encodedData.data(), static_cast<uint32_t>(encodedData.size()));
    }

    vector<uint8_t> Encode(
        const string&  str,
        uint32_t       fourcc,
        ICompressor*   pCompressor,
        const uint8_t* pKey,
        const void*    pIV)
    {
        return Encode(str.c_str(), static_cast<int32_t>(str.size() + 1), fourcc, pCompressor, pKey, pIV);
    }

    void Encode(
        const string&    str,
        vector<uint8_t>& encodedData,
        uint32_t         fourcc,
        ICompressor*     pCompressor,
        const uint8_t*   pKey,
        const void*      pIV)
    {
        return Encode(str.c_str(), static_cast<int32_t>(str.size() + 1), encodedData, fourcc, pCompressor, pKey, pIV);
    }

    void Encode(
        const fs::path& pathname,
        const string&   str,
        uint32_t        fourcc,
        ICompressor*    pCompressor,
        const uint8_t*  pKey,
        const void*     pIV)
    {
        return Encode(pathname, str.c_str(), static_cast<int32_t>(str.size() + 1), fourcc, pCompressor, pKey, pIV);
    }

    vector<uint8_t> Decode(const fs::path& pathname, uint32_t expectedFourcc, int32_t maxLen, const uint8_t* pKey)
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

            if (fileSize < 2)
            {
                throw XceptionFile(pathname, 0, "File is too small.");
            }

            if (!data.empty() && data.front() != ASCII::LeftBrace)
            {
                data = Decode(data.data(), data.size(), expectedFourcc, maxLen, pKey);
            }
        }

        return data;
    }

    vector<uint8_t> Decode(void* pContainer, size_t containerLen, uint32_t expectedFourcc, int32_t maxLen, const uint8_t* pKey)
    {
        vector<uint8_t> contents;

        if (containerLen)
        {
            if (!pContainer || maxLen < 0 || containerLen > static_cast<size_t>(maxLen))
            {
                throw Xception(0, "Invalid or corrupt data.");
            }

            if (*reinterpret_cast<char*>(pContainer) == ASCII::LeftBrace)
            {
                contents.reserve(containerLen + 1);
                contents.resize(containerLen);
                memcpy(contents.data(), pContainer, containerLen);
                contents.push_back(0);
                contents.pop_back();
            }
            else
            {
                FileHdr* pHdr = static_cast<FileHdr*>(pContainer);
                if ((expectedFourcc && pHdr->fourcc != expectedFourcc) || !pHdr->isSizeValid(maxLen)) //-V106
                {
                    throw Xception(0, "Invalid or corrupt data.");
                }

                ContentHdr* pContentHdr = reinterpret_cast<ContentHdr*>(&pHdr[1]);

                if (pHdr->contentLen > 0)
                {
                    IvTag* pIvTag = reinterpret_cast<IvTag*>(reinterpret_cast<uint8_t*>(pContentHdr) + pHdr->contentLen); //-V104

                    const int result = AES_GCM::decrypt(pKey, pIvTag->iv, nullptr, reinterpret_cast<uint8_t*>(pContentHdr), pHdr->contentLen, //-V106
                                                        pIvTag->tag, pHdr, sizeof(FileHdr));
                    if (result != pHdr->contentLen)
                    {
                        throw Xception(0, "Invalid or corrupt data.");
                    }
                }
                else
                {
                    pHdr->contentLen = -pHdr->contentLen;
                }

                if (pContentHdr->uncompressedLen > static_cast<uint32_t>(maxLen))
                {
                    throw Xception(0, "Invalid or corrupt data.");
                }

                DecompressFunction decompressFunction;

                const uint32_t compressedLen = static_cast<uint32_t>(pHdr->contentLen) - sizeof(ContentHdr); //-V104 //-V103

                switch (pContentHdr->fourcc)
                {
                    case 0:
                        if (compressedLen != pContentHdr->uncompressedLen)
                        {
                            throw Xception(0, "Invalid or corrupt data.");
                        }
                        decompressFunction = CopyCompressor::Decompress;
                        break;

                    case MakeFourCC('L', 'Z', '4', 0):
                        decompressFunction = LZ4Compressor::Decompress;
                        break;

                    default:
                        throw Xception(0, "Invalid or corrupt data.");
                }

                contents.resize(pContentHdr->uncompressedLen);      //-V106
                const int32_t decompressedLen = decompressFunction(&pContentHdr[1], static_cast<int>(compressedLen),
                                                                   contents.data(), static_cast<int>(contents.size()));
                if (decompressedLen != static_cast<int32_t>(contents.size()))
                {
                    throw Xception(0, "Invalid or corrupt data.");
                }
            }
        }

        return contents;
    }
}