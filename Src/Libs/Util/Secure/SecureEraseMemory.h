// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

/** Securely erases memory.
   @param pDst The memory to erase.
   @param len The number of bytes of memory to erase.
   @remarks memset may be optimized out by the compiler. SecureEraseMemory
        ensures that the memory will be set to zero.
 */
void SecureEraseMemory(void* pDst, size_t len) noexcept;
