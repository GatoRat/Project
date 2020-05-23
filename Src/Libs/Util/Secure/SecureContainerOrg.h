// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace SecureContainerOrg
{
    enum class EncryptionType : uint32_t
    {
        None    = 0x00,
        AES_GCM = 0x01,
    };

    enum class CompressionType : uint32_t
    {
        None = 0x0000,
        LZ4  = 0x0001,
        //LZMA = 0x0002,
    };

    /////////////////////////////////////////////////////////////////////////

    typedef std::function<int32_t(const void* pSrc, int32_t srcLen, void* pDst, int32_t dstLen)> DecompressFunction;

    struct ICompressor
    {
        ICompressor() = default;
        virtual ~ICompressor() = default;

        virtual CompressionType getType() const = 0;
        virtual uint32_t getFourCC() const = 0;

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
        CompressionType getType() const override;
        uint32_t        getFourCC() const override;

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

        CompressionType getType() const override;
        uint32_t        getFourCC() const override;
        int32_t         calcMaxOutputSize(int32_t dataLen) const override;
        int32_t         compress(const void* pSrc, int32_t srcLen, void* pDst, int32_t maxOutputSize) const override;

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

#pragma pack(push, 1)

    struct FileHdr
    {
        uint32_t fourcc;
        int32_t  contentLen;

        bool isSizeValid(size_t size) const;
    };

#pragma pack(pop)

    /////////////////////////////////////////////////////////////////////////

    void Encode(const void* pData, int32_t dataLen, std::vector<uint8_t>& encodedData, uint32_t fourcc, ICompressor* pCompressor, const uint8_t* pKey, const void* pIV = nullptr);
    void Encode(const std::string& str, std::vector<uint8_t>& encodedData, uint32_t fourcc, ICompressor* pCompressor, const uint8_t* pKey, const void* pIV = nullptr);

    std::vector<uint8_t> Encode(const void* pData, int32_t dataLen, uint32_t fourcc, ICompressor* pCompressor, const uint8_t* pKey, const void* pIV = nullptr);
    void                 Encode(const fs::path& pathname, const void* pData, int32_t dataLen, uint32_t fourcc, ICompressor* pCompressor, const uint8_t* pKey, const void* pIV = nullptr);

    std::vector<uint8_t> Encode(const std::string& str, uint32_t fourcc, ICompressor* pCompressor, const uint8_t* pKey, const void* pIV = nullptr);
    void                 Encode(const fs::path& pathname, const std::string& str, uint32_t fourcc, ICompressor* pCompressor, const uint8_t* pKey, const void* pIV = nullptr);

    /** Loads and decodes a SecureContainer file.
        @param pathname The filename to load.
        @param expectedFourcc Expected FOURCC. If zero, no check is made.
        @param key A pointer to the expected AES GCM key. May be nullptr if no encryption is expected.
        @returns The decoded contents of the SecureContainer file.
    */
    std::vector<uint8_t> Decode(const fs::path& pathname, uint32_t expectedFourcc, int32_t maxLen, const uint8_t* pKey);

    /** Decodes a SecureContainer.
        @param pContainer A pointer to the data. This buffer may be modified.
        @param containerLen The length of the data, in bytes.
        @param expectedFourcc Expected FOURCC. If zero, no check is made.
        @param key A pointer to the expected AES GCM key. May be nullptr if no encryption is expected.
        @returns The decoded contents of the SecureContainer.
    */
    std::vector<uint8_t> Decode(void* pContainer, size_t containerLen, uint32_t expectedFourcc, int32_t maxLen, const uint8_t* pKey);
}
