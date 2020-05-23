// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/*
 *  FIPS-197 compliant AES implementation
 *  Code by Christophe_Devine.
 *  Website: http://www.cr0.net:8040/code/crypto/aes/
 *  See disclaimer in aes.cpp.
 */

#pragma once

#include <cstdint>
#include "SecureBuffer.h"

class AES final
{
public:
    static constexpr size_t KeyLen    = 32; //-V112
    static constexpr size_t BlockSize = 16;

public:
    AES() = default;
    AES(const void* pKey, size_t keyLen, bool encryptOnly = false);
    AES(const SecureBuffer<KeyLen>& key, bool encryptOnly = false);
    AES(const AES&) = delete;
    void operator=(const AES&) = delete;
    ~AES();

    void setKey(const void* pKey, size_t keyLen, bool encryptOnly = false);
    void setKey(const SecureBuffer<KeyLen>& key, bool encryptOnly = false);

    /** Encrypt buffer.
       @param pInput Pointer to the data to encrypt.
       @param pOutput Pointer the buffer to receive the encrypted data.
            pOutput can be the same as pInput.
       @param dataLen Length of the data, in bytes, of pInput \b AND pOutput.
            This \b MUST be a multiple of BlockSize (16 bytes).
       @returns \b true on success, else \b false. Only bad parameters will
            return \b false.
     */
    bool encrypt(const void* pInput, void* pOutput, size_t dataLen);
    bool decrypt(const void* pInput, void* pOutput, size_t dataLen);

    void encryptBlock(const uint8_t input[BlockSize], uint8_t output[BlockSize]);
    void decryptBlock(const uint8_t input[BlockSize], uint8_t output[BlockSize]);

private:
    uint32_t erk[64]; // encryption round keys
    uint32_t drk[64]; // decryption round keys
};
