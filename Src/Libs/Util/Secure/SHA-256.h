#pragma once

#include <cstdint>
#include <cstring>

struct SHA256_context
{
    uint32_t total[2];
    uint32_t state[8];
    uint8_t  buffer[64];
};

struct SHA256 final
{
public:
    static constexpr size_t DigestLen = 32; //-V112

    struct Digest
    {
        uint8_t digest[DigestLen];       //!< MD5 digest as 8-bit values.

        bool operator==(const Digest& other) const
        {
            return memcmp(digest, other.digest, sizeof(digest));
        }
    };

private:
    SHA256_context context;

public:
    SHA256();
    SHA256(const void* pData, uint32_t dataLen, Digest& digest);
    SHA256(const SHA256&) = delete;
    void operator=(const SHA256&) = delete;
    ~SHA256() = default;

    void init();
    void update(const void* pData, uint32_t dataLen);
    void final(Digest& digest);

    static void calc(const void* pData, uint32_t dataLen, Digest& digest);
};
