// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "VerifyFormat.Sound.h"
#include <cstddef>
#include <cstdint>
#include "../File/FastFile.h"
#include "../Misc/MakeFourCC.h"

using namespace std;

#pragma pack(push, 1)

struct Wav
{
    struct ChunkHdr
    {
        uint32_t fourcc;
        uint32_t size; // doesn't include this header
    };

    struct
    {
        ChunkHdr hdr;       // "RIFF", size of file - 8
        uint32_t wave_4cc;  // "WAVE"

        bool verify(uint64_t fileSize) const
        {
            return fileSize < UINT32_MAX &&
                hdr.size == static_cast<uint32_t>(fileSize - sizeof(ChunkHdr)) &&
                hdr.fourcc == MakeFourCC('R', 'I', 'F', 'F') &&
                wave_4cc == MakeFourCC('W', 'A', 'V', 'E');
        }

    } riffChunk;

    struct
    {
        ChunkHdr hdr;           // "fmt ", 16
        uint16_t format;        // 1
        uint16_t numChannels;   // 1 or 2
        uint32_t sampleRate;    // In range of 8000-48000 (this actually could be anything)
        uint32_t byteRate;      // numChannels * sampleRate * bitsPerSample/8
        uint16_t blockAlign;    // numChannels * bitsPerSamples/8
        uint16_t bitsPerSample; // 8, 16 or 32

        bool verify() const
        {
            return hdr.fourcc == MakeFourCC('f', 'm', 't', ' ') &&
                hdr.size == 16 &&
                format == 1 &&
                (numChannels == 1 || numChannels == 2) &&
                (sampleRate >= 8000 && sampleRate <= 48000) &&
                (bitsPerSample == 8 || bitsPerSample == 16) &&
                (blockAlign == static_cast<uint16_t>(numChannels * (bitsPerSample / 8))) &&
                (byteRate == static_cast<uint32_t>(sampleRate * numChannels * (bitsPerSample / 8)));
        }

    } fmtChunk;

    /*
        Generally, the data follows. However, some files may have addition chunks & subchunks, such as:
        fact, cue, playlist, associated data, LIST, slnt, INFO, CSET. Unfortunately, the standard doesn't
        specify their order. In a full check, each chunk would be at least verified as having a correct
        size.
    */

    bool verify(uint64_t fileSize) const
    {
        return riffChunk.verify(fileSize) && fmtChunk.verify();
    }
};

#pragma pack(pop)

bool VerifyWAV(const uint8_t* pData, size_t dataLen, bool full)
{
    bool success = false;

    if (pData && dataLen >= sizeof(Wav))
    {
        const Wav* pWav{ reinterpret_cast<const Wav*>(pData) };
        success = pWav->verify(dataLen);
        if (success && full)
        {
            bool hasDataChunk = false;

            dataLen -= sizeof(Wav);
            pData += sizeof(Wav);

            do
            {
                if (dataLen < sizeof(Wav::ChunkHdr))
                {
                    success = false;
                }
                else
                {
                    const Wav::ChunkHdr* pChunk{ reinterpret_cast<const Wav::ChunkHdr*>(pData) };

                    dataLen -= sizeof(Wav::ChunkHdr);
                    pData += sizeof(Wav::ChunkHdr);

                    if (pChunk->size > dataLen) //-V104
                    {
                        success = false;
                    }
                    else
                    {
                        if (pChunk->fourcc == MakeFourCC('d', 'a', 't', 'a'))
                        {
                            hasDataChunk = true;
                        }

                        dataLen -= pChunk->size; //-V101
                        pData += pChunk->size; //-V102
                    }
                }

            } while (success && dataLen);

            if (!hasDataChunk)
            {
                success = false;
            }
        }
    }

    return success;
}

bool VerifyWAV(const vector<uint8_t>& data, bool full)
{
    return VerifyWAV(data.data(), data.size(), full);
}

bool VerifyWAV(const fs::path& pathname, bool full, uint32_t maxWavFileLen)
{
    bool success = false;

    int64_t fileSize = 0;

    if (full && maxWavFileLen > sizeof(Wav))
    {
        vector<uint8_t> data = FastReadFile(pathname, maxWavFileLen, &fileSize);
        if (data.size() == static_cast<size_t>(fileSize))
        {
            success = VerifyWAV(data, full);
        }
    }
    else
    {
        Wav wav;
        const size_t lenRead = FastReadFile(pathname, &wav, sizeof(wav), &fileSize); //-V101
        if (lenRead == sizeof(wav))
        {
            success = wav.verify(fileSize);
        }
    }

    return success;
}
