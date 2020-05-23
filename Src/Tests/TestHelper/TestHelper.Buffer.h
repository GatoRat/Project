// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <array>
#include <cstdint>

namespace TestHelper
{
    /** Fills a buffer with a [repeating] sequence of characters.
        @param pBuffer   The buffer to which to write the characters.
        @param bufferLen The number of character to write.
        @param startChar The start character of the [repeating] sequence.
        @param endChar   The end character of the [repeating] sequence.
        @param terminate If \b true, contents will be terminated with a zero.
     */
    char* FillBuffer(char* pBuffer, size_t fillLen, char startChar = '0', char endChar = 'z', bool terminate = false);

    /** Checks if a buffer contains the correct [repeating] sequence of characters.
        @param pBuffer   The buffer to check.
        @param bufferLen The number of character to check.
        @param startChar The start character of the [repeating] sequence.
        @param endChar   The end character of the [repeating] sequence.
        @param terminate If \b true, contents are expected to be terminated with a zero.
     */
    bool CheckBuffer(const char* pBuffer, size_t checkLen, char startChar = '0', char endChar = 'z', bool terminate = false);

    template<size_t bufferLen>
    class Buffer
    {
    public:
        explicit Buffer(bool doFill = true)
        {
            if (doFill)
            {
                fill();
            }
            else
            {
                _buffer.fill(0);
            }
        }

        Buffer(char startChar, char endChar, bool terminate = false)
            : _startChar(startChar)
            , _endChar(endChar)
            , _terminate(terminate)
        {
            fill();
        }

        Buffer(const Buffer& other) = default;
        Buffer& operator=(const Buffer& other) = default;
        ~Buffer() = default;

        size_t size() const
        {
            return _buffer.size();
        }

        operator void*()
        {
            return _buffer.data();
        }

        operator uint8_t*()
        {
            return static_cast<uint8_t*>(_buffer.data());
        }

        char* fill()
        {
            return FillBuffer(_buffer.data(), _buffer.size(), _startChar, _endChar, _terminate);
        }

        bool check()
        {
            return CheckBuffer(_buffer.data(), _buffer.size(), _startChar, _endChar, _terminate);
        }

        bool compare(const uint8_t* pData, size_t index = 0, size_t len = SIZE_MAX)
        {
            bool result = false;
            if (index < _buffer.size())
            {
                len = std::min(len, _buffer.size() - index);
                result = memcmp(pData, &_buffer.data()[index], len) == 0;
            }
            return result;
        }

    public:
        std::array<char, bufferLen> _buffer;

    private:
        char _startChar = '0';
        char _endChar   = 'z';
        bool _terminate = false;
    };
}
