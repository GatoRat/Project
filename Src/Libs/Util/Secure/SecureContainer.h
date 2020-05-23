// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace SecureContainer
{
    constexpr int32_t MinCompressLen = 512;

    constexpr uint8_t NoCompression  = 0;
    constexpr uint8_t LZ4Compressed  = 1;

    ///////////////////////////////////////////////////////////////////////////

    typedef std::function<int32_t(const void* pSrc, int32_t srcLen, void* pDst, int32_t dstLen)> DecompressFunction;

    struct ICompressor
    {
        ICompressor() = default;
        virtual ~ICompressor() = default;

        virtual uint8_t getType() const = 0;

        /** Returns that maximum memory size needed for "worst case" scenario.
            @dataLen The length of the data to compress. This will never be zero (0).
            @returns The maximum memory size needed for "worst case" scenario (data
                not compressible).
            @remarks DoCallback size must be >= dataLen.
        */
        virtual int32_t calcMaxOutputSize(int32_t dataLen) const = 0;
        virtual int32_t compress(const void* pSrc, int32_t srcLen, void* pDst, int32_t maxOutputSize) const = 0;
    };

    /////////////////////////////////////////////////////////////////////////

    struct CopyCompressor : public ICompressor
    {
        uint8_t getType() const override;

        int32_t calcMaxOutputSize(int32_t dataLen) const override;
        int32_t compress(const void* pSrc, int32_t srcLen, void* pDst, int32_t maxOutputSize) const override;

        static int32_t Decompress(const void* pSrc, int32_t srcLen, void* pDst, int32_t /*maxOutputSize*/);
    };

    /////////////////////////////////////////////////////////////////////////

    struct LZ4Compressor : public ICompressor
    {
        static constexpr int DefaultAcceleration = 1;

        union
        {
            int acceleration;
            int level;
        };
        /** LZ4Compressor constructor.
            @param level The compression level. If less than LZ4HC_CLEVEL_MIN (3), will
                use LZ4_compress_fast with level passed for accelerator parameter.
        */
        explicit LZ4Compressor(int acceleration = DefaultAcceleration);

        uint8_t getType() const override;
        int32_t calcMaxOutputSize(int32_t dataLen) const override;
        int32_t compress(const void* pSrc, int32_t srcLen, void* pDst, int32_t maxOutputSize) const override;

        static int32_t Decompress(const void* pSrc, int32_t srcLen, void* pDst, int32_t dstLen);
    };

    struct LZ4HighCompressor : public LZ4Compressor
    {
        static constexpr int MinLevel     = 3;
        static constexpr int DefaultLevel = 10; // 10 tends to be faster than 9 with slightly better compression.
        static constexpr int MaxLevel     = 12; // Levels 11 and 12 will give a few percent better compression at a horrendous cost in time

        explicit LZ4HighCompressor(int level = DefaultLevel);

        int32_t compress(const void* pSrc, int32_t srcLen, void* pDst, int32_t maxOutputSize) const override;
    };

    /////////////////////////////////////////////////////////////////////////

    void Encode(const void* pData, int32_t dataLen, std::vector<uint8_t>& encodedData, const uint8_t* pKey, ICompressor* pCompressor, int32_t minCompressLen = MinCompressLen);
    void Encode(const std::string& str, std::vector<uint8_t>& encodedData, const uint8_t* pKey, ICompressor* pCompressor, int32_t minCompressLen = MinCompressLen);

    std::vector<uint8_t> Encode(const void* pData, int32_t dataLen, const uint8_t* pKey, ICompressor* pCompressor, int32_t minCompressLen = MinCompressLen);
    void                 Encode(const fs::path& pathname, const void* pData, int32_t dataLen, const uint8_t* pKey, ICompressor* pCompressor, int32_t minCompressLen = MinCompressLen);

    std::vector<uint8_t> Encode(const std::string& str, const uint8_t* pKey, ICompressor* pCompressor, int32_t minCompressLen = MinCompressLen);
    void                 Encode(const fs::path& pathname, const std::string& str, const uint8_t* pKey, ICompressor* pCompressor, int32_t minCompressLen = MinCompressLen);

    /** Loads and decodes a SecureContainer file.
        @param pathname The filename to load.
        @param pAesKey A pointer to the AES key. May be nullptr if no encryption is expected.
        @returns The decoded contents of the SecureContainer file.
    */
    std::vector<uint8_t> Decode(const fs::path& pathname, int32_t maxLen, const uint8_t* pKey);

    /** Decodes a SecureContainer.
        @param pContainer A pointer to the data. This buffer may be modified.
        @param containerLen The length of the data, in bytes.
        @param pAesKey A pointer to the AES key. May be nullptr if no encryption is expected.
        @returns The decoded contents of the SecureContainer.
    */
    std::vector<uint8_t> Decode(void* pContainer, size_t containerLen, int32_t maxLen, const uint8_t* pKey);

    void Decode(void* pContainer, size_t containerLen, int32_t maxLen, const uint8_t* pKey, const std::function<void*(int32_t finalLen)>& getFinalBuffer);

    template<class Collection>
    Collection Decode(void* pContainer, size_t containerLen, int32_t maxLen, const uint8_t* pKey)
    {
        Collection collection;

        Decode(pContainer, containerLen, maxLen, pKey, [&](int32_t finalLen)->void*
        {
            collection.resize(finalLen);
            return collection.data();
        });

        return collection;
    }
}
