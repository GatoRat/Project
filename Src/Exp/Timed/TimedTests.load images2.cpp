// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>
#include "Util/Detect/DetectCompiler.h"
#include "Util/Misc/Defines.Misc.h"
#include "Util/Misc/TimedTest.h"

namespace fs = std::filesystem;

#include "Util/Graphics/WICImage.h"
//#include "Util/String/FormatSizeFast.h"
#include <iostream>
//#include "Util/System/SysInfo.h"
#include <SFML/Graphics.hpp>
#include "Util/File/FastFile.h"
#include <lz4/lz4hc.h>

using namespace std;

#undef  OPTIMIZE_OFF
#define OPTIMIZE_OFF

#undef  OPTIMIZE_ON
#define OPTIMIZE_ON

#ifdef _MSC_VER
    #pragma warning(disable : 4748) // /GS can not protect parameters and local variables from local buffer overrun because optimizations are disabled in function
#endif


#include "Util/Graphics/Graphics.h"
#include "Util/Graphics/Image.h"
#include "Util/Misc/MakeFourCC.h"
#include "Util/Misc/Defines.Misc.h"
#include "Util/Secure/Crc32c.h"

namespace PavImage
{
    constexpr uint16_t MaxWidth = 16 * ONE_KILOBYTE;
    constexpr uint16_t MaxHeight = 16 * ONE_KILOBYTE;

#pragma pack(push, 1)

    struct Hdr
    {
        uint32_t crc32c;     // Set to zero and include entire data block
        uint32_t fourcc;     // GAGA - uses LZ4 and this 16-byte header
        uint16_t width;
        uint16_t height;

        void set(uint32_t width_, uint32_t height_)
        {
            crc32c = 0;
            fourcc = MakeFourCC('P', 'A', 'V', 'I');
            width = static_cast<uint16_t>(width_);
            height = static_cast<uint16_t>(height_);
        }

        bool verify() const
        {
            return fourcc == MakeFourCC('P', 'A', 'V', 'I') && width && width <= MaxWidth && height && height <= MaxHeight;
        }
    };

#pragma pack(pop)

    vector<char> Compress(uint32_t width, uint32_t height, const uint8_t* pPixelsRgba, uint32_t maxWidth = MaxWidth, uint32_t maxHeight = MaxHeight)
    {
        vector<char> dst;

        if (width && width <= maxWidth && height && height <= maxHeight && pPixelsRgba)
        {
            const int srcSize = static_cast<int>(width * height * sizeof(Graphics::Pixel)); //-V104
            const int dstCapacity = LZ4_compressBound(srcSize);
            dst.resize(sizeof(Hdr) + dstCapacity);

            Hdr* pHdr = reinterpret_cast<Hdr*>(dst.data());
            pHdr->set(width, height);

            int compressedLen = LZ4_compress_HC(reinterpret_cast<const char*>(pPixelsRgba), dst.data() + sizeof(Hdr),
                srcSize, dstCapacity, LZ4HC_CLEVEL_MAX);
            if (compressedLen > 0)
            {
                dst.resize(sizeof(Hdr) + compressedLen); //-V104
            }
            else
            {
                dst.resize(0);
            }
        }

        return dst;
    }

    Graphics::Image Decompress(const void* pData, size_t dataLen)
    {
        Graphics::Image image;

        if (dataLen > sizeof(Hdr))
        {
            const Hdr* pHdr = reinterpret_cast<const Hdr*>(pData);
            const int dstSize = static_cast<int>(pHdr->width * pHdr->height * sizeof(Graphics::Pixel));
            if (dstSize)
            {
                image.resize(pHdr->width, pHdr->height);

                int decompressedLen = LZ4_decompress_safe(reinterpret_cast<const char*>(&pHdr[1]), reinterpret_cast<char*>(image.getWritableData()),
                    static_cast<int>(dataLen - sizeof(Hdr)), static_cast<int>(image.getDataLen()));
                if (decompressedLen <= 0)
                {
                    image.reset();
                }

                //if (decompressedLen != dstSize)
                //{
                //    cout << "!!!!!!! Error: " << entry.path() << endl;
                //}
                //else
                //{
                //    totalSize += dst.size();
                //}
            }
        }

        return image;
    }

    Graphics::Image Decompress(const vector<char>& pavImage)
    {
        return Decompress(pavImage.data(), pavImage.size());
    }

    Graphics::Image Decompress(const vector<uint8_t>& pavImage)
    {
        return Decompress(pavImage.data(), pavImage.size());
    }
}

struct TestAll : public TimedTest
{
    const char* GetName() override { return "All"; }

    bool InitTest() override
    {
        return true;
    }

    bool PreTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void Test() override
    {
    }
    OPTIMIZE_ON

    bool PostTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void DeinitTest() override
    {
    }
    OPTIMIZE_ON
};

struct FileData
{
    fs::path pathname;
    vector<uint8_t> data;

    explicit FileData(const fs::path& pathname_)
        : pathname(pathname_)
    {
        data = FastReadFile(pathname);
    }
};

vector<FileData> files;

struct Test1 : public TimedTest
{
    const char* GetName() override { return "Test1"; }

    bool InitTest() override
    {
        return true;
    }

    bool PreTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void Test() override
    {
        //{
        //    ProcessMemory processMemory;

        //    cout << "System memory: " << GetMemoryString() << endl;

        //    cout << "Process memory: " << FormatMemorySizeFast(processMemory.getWorkingSetSize()) <<
        //        ", Process page usage: " << FormatMemorySizeFast(processMemory.getPagefileUsage()) << endl;
        //}

        //uint64_t totalSize = 0;
        //vector<WICImaging::ImageData> data;

        files.clear();

        for (auto& entry : fs::recursive_directory_iterator("C:/Temp/SuperPick.Western"))
        {
            if (entry.path().extension().generic_wstring() == L".png") // L".pavi")
            {
                files.emplace_back(entry.path());
            }
        }

        //cout << "Western: " << FormatMemorySizeFast(totalSize) << endl;
        //{
        //    ProcessMemory processMemory;

        //    cout << "System memory: " << GetMemoryString() << endl;

        //    cout << "Process memory: " << FormatMemorySizeFast(processMemory.getWorkingSetSize()) <<
        //        ", Process page usage: " << FormatMemorySizeFast(processMemory.getPagefileUsage()) << endl;
        //}
    }
    OPTIMIZE_ON

    virtual bool PostTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void DeinitTest() override
    {
    }
    OPTIMIZE_ON
};

//vector<vector<char>> images;

vector<sf::Image> images;

struct Test2 : public TimedTest
{
    const char* GetName() override { return "Test2"; }

    bool InitTest() override
    {
        return true;
    }

    bool PreTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void Test() override
    {
        images.clear();

        //for (auto& file : files)
        //{
        //    images.emplace_back();

        //    Graphics::Image image = PavImage::Decompress(file.data);

        //    images.back().create(image.getWidth(), image.getHeight(), image.getData());
        //}

        for (auto& file : files)
        {
            images.emplace_back();
            images.back().loadFromMemory(file.data.data(), file.data.size());
        }

/*
        for (auto& entry : fs::recursive_directory_iterator("C:/Temp/SuperPick.Western"))
        {
            if (entry.path().extension().native() == L".png")
            {
                WICImaging::ImageData image(entry.path());

                vector<char> dst = PavImage::Compress(image.getWidth(), image.getHeight(), image.getPixelPtr());

                if (dst.empty())
                {
                    cout << "!!!!!!! Error: " << entry.path() << endl;
                }
                else
                {
                    fs::path dstFile(entry.path());
                    dstFile.replace_extension(".pavi");
                    FastWriteFile(dstFile, dst.data(), static_cast<uint32_t>(dst.size()));
                }
            }
        }
*/
        //for (auto& entry : fs::recursive_directory_iterator("C:/Temp/SuperPick.Western.lz"))
        //{
        //    if (entry.path().extension().generic_wstring() == L".lz4i")
        //    {
        //        auto src = FastReadFile(entry.path());
        //        struct Hdr
        //        {
        //            uint32_t width;
        //            uint32_t height;
        //        };

        //        const Hdr* pHdr = reinterpret_cast<const Hdr*>(src.data());

        //        vector<char> dst;
        //        dst.resize(pHdr->width * pHdr->height * 4);

        //        int decompressedLen = LZ4_decompress_safe(reinterpret_cast<const char*>(src.data() + sizeof(Hdr)), dst.data(), static_cast<int>(src.size() - sizeof(Hdr)), static_cast<int>(dst.size()));
        //        if (decompressedLen != static_cast<int>(dst.size()))
        //        {
        //            cout << "!!!!!!! Error: " << entry.path() << endl;
        //        }
        //        else
        //        {
        //            totalSize += dst.size();
        //        }
        //    }
        //}

        //cout << "Western (lz4): " << FormatMemorySizeFast(totalSize) << endl;


            //
            //                Hdr* pHdr = reinterpret_cast<Hdr*>(dst.data());
            //                pHdr->width  = image.getWidth();
            //                pHdr->height = image.getHeight();
            //
            //                int compressedLen = LZ4_compress_HC(reinterpret_cast<const char*>(image.getRawPixels()), dst.data() + sizeof(Hdr),
            //                    static_cast<int>(image.getPixels().size() * 4), static_cast<int>(dst.size() - sizeof(Hdr)), LZ4HC_CLEVEL_MAX);
            //
            //                if (!compressedLen)
            //                {
            //                    cout << "!!!!!!! Error: " << entry.path() << endl;
            //                }
            //                else
            //                {
            //                    fs::path dstFile(entry.path());
            //                    dstFile.replace_extension(".lz4i");
            //                    FastWriteFile(dstFile, dst.data(), compressedLen + sizeof(Hdr));
            //                }
            ////                totalSize += data.size();
            //            }
            //        }
            //        cout << "Western (raw): " << FormatMemorySizeFast(totalSize) << endl;



/*
        {
            ProcessMemory processMemory;

            cout << "System memory: " << GetMemoryString() << endl;

            cout << "Process memory: " << FormatMemorySizeFast(processMemory.getWorkingSetSize()) <<
                ", Process page usage: " << FormatMemorySizeFast(processMemory.getPagefileUsage()) << endl;
        }

        uint64_t totalSize = 0;
        vector<WICImaging::ImageData> data;

        for (auto& entry : fs::recursive_directory_iterator("C:/Temp/SuperPick.Islands"))
        {
            if (entry.path().extension().generic_wstring() == L".png")
            {
                data.emplace_back(entry.path().c_str());
                totalSize += data.back().getWidth() * data.back().getHeight() * sizeof(uint32_t);
            }
        }


        cout << "Islands: " << FormatMemorySizeFast(totalSize) << endl;
        {
            ProcessMemory processMemory;

            cout << "System memory: " << GetMemoryString() << endl;

            cout << "Process memory: " << FormatMemorySizeFast(processMemory.getWorkingSetSize()) <<
                ", Process page usage: " << FormatMemorySizeFast(processMemory.getPagefileUsage()) << endl;
        }

        //uint64_t totalSize = 0;

        //vector<fs::path> files;
        //for (auto& entry : fs::recursive_directory_iterator("C:/Temp/SuperPick.Islands"))
        //{
        //    if (entry.path().extension().generic_wstring() == L".png")
        //    {
        //        WICImaging::ImageData image(entry.path().c_str());
        //        totalSize += image.getWidth() * image.getHeight() * sizeof(uint32_t);
        //    }
        //}

        //cout << "Islands: " << FormatMemorySizeFast(totalSize) << endl;
*/
    }
    OPTIMIZE_ON

    virtual bool PostTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void DeinitTest() override
    {
    }
    OPTIMIZE_ON
};

#include "Util/Misc/Stopwatch.h"
#include "Util/String/FormatSizeFast.h"

struct Test3 : public TimedTest
{
    const char* GetName() override { return "Test3"; }

    bool InitTest() override
    {
        return true;
    }

    bool PreTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void Test() override
    {
        sf::Image image;
        image.loadFromFile("C:/Temp/SuperPick.Western/Miner.Dance.png");

        vector<sf::Texture> textures;
        uint64_t totalBytes = 0;
        Stopwatch stopwatch;

        for (int i = 0; i < 25; ++i)
        {
            textures.emplace_back();
            textures.back().loadFromImage(image);

            totalBytes += static_cast<uint64_t>(image.getSize().x * image.getSize().y * 4);
        }

        double elapsed = stopwatch.GetSplit();
        uint64_t byesPerSecond = static_cast<uint64_t>(totalBytes / elapsed);

        cout << FormatMemorySizeFast(byesPerSecond) << " per second" << " (total=" << FormatMemorySizeFast(totalBytes) << ")" <<
            endl;


        //for (auto& image : images)
        //{
        //    textures.emplace_back();
        //    textures.back().loadFromImage(image);
        //}

        //for (auto& entry : fs::recursive_directory_iterator("C:/Temp/SuperPick.Western"))
        //{
        //    if (entry.path().extension().generic_wstring() == L".png") // L".pavi")
        //    {
        //        sf::Image image;
        //        image.loadFromFile(entry.path().string().c_str());
        //        textures.emplace_back();
        //        textures.back().loadFromImage(image);
        //    }
        //}


        //for (auto& file : files)
        //{
        //    sf::Image image;
        //    image.loadFromMemory(file.data.data(), file.data.size());
        //    textures.emplace_back();
        //    textures.back().loadFromImage(image);

        ////    //Graphics::Image image = PavImage::Decompress(file.data);

        ////    //textures.emplace_back();
        ////    //textures.back().create(image.getWidth(), image.getHeight());
        ////    //textures.back().update(image.getData());
        //}

/*
        {
            ProcessMemory processMemory;

            cout << "System memory: " << GetMemoryString() << endl;
            cout << "Process memory: " << FormatMemorySizeFast(processMemory.getWorkingSetSize()) <<
                ", Process page usage: " << FormatMemorySizeFast(processMemory.getPagefileUsage()) << endl;
        }

        uint64_t totalSize = 0;

        for (auto& entry : fs::recursive_directory_iterator("C:/Temp/SuperPick.Western"))
        {
            if (entry.path().extension().native() == L".pavi")
            {
                auto img = FastReadFile2(entry.path());
                images.emplace_back(move(img));
                totalSize += img.size();
            }
        }

        cout << "Western: " << FormatMemorySizeFast(totalSize) << endl;
        {
            ProcessMemory processMemory;

            cout << "System memory: " << GetMemoryString() << endl;
            cout << "Process memory: " << FormatMemorySizeFast(processMemory.getWorkingSetSize()) <<
                ", Process page usage: " << FormatMemorySizeFast(processMemory.getPagefileUsage()) << endl;
        }
*/

/*
        {
            ProcessMemory processMemory;

            cout << "System memory: " << GetMemoryString() << endl;

            cout << "Process memory: " << FormatMemorySizeFast(processMemory.getWorkingSetSize()) <<
                ", Process page usage: " << FormatMemorySizeFast(processMemory.getPagefileUsage()) << endl;
        }

        uint64_t totalSize = 0;

        for (auto& entry : fs::recursive_directory_iterator("C:/Temp/SuperPick.Western"))
        {
            if (entry.path().extension().generic_wstring() == L".png")
            {
                images.emplace_back();
                images.back().loadFromFile(entry.path().string().c_str());
                uint64_t size = images.back().getSize().x * images.back().getSize().y * sizeof(uint32_t);
                totalSize += size;
            }
        }

        cout << "Western: " << FormatMemorySizeFast(totalSize) << endl;
        {
            ProcessMemory processMemory;

            cout << "System memory: " << GetMemoryString() << endl;

            cout << "Process memory: " << FormatMemorySizeFast(processMemory.getWorkingSetSize()) <<
                ", Process page usage: " << FormatMemorySizeFast(processMemory.getPagefileUsage()) << endl;
        }
*/
    }
    OPTIMIZE_ON

    virtual bool PostTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void DeinitTest() override
    {
    }
    OPTIMIZE_ON
};

struct Test4 : public TimedTest
{
    const char* GetName() override { return "Test4"; }

    bool InitTest() override
    {
        return true;
    }

    bool PreTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void Test() override
    {
        vector<sf::Texture> textures;

        uint64_t totalBytes = 0;
        Stopwatch stopwatch;

        for (auto& image : images)
        {
            textures.emplace_back();
            auto& texture = textures.back();// .loadFromImage(image);

            //    bool create2(unsigned int width, unsigned int height,
            //int internalFormat = 0x1908, unsigned pixelFormat = 0x1908, unsigned pixelType = 0x1401);
            //texture.create2(image.getSize().x, image.getSize().y, 0x8058, 0x80E1, 0x8367);

            //void update2(const void* pixels, unsigned width, unsigned height, unsigned x, unsigned y,
            //    unsigned pixelFormat = 0x1908, unsigned pixelType = 0x1401);

            // 0x8058, 0x80E1, 0x8367
            //texture.update2(image.getPixelsPtr(), image.getSize().x, image.getSize().y, 0, 0, 0x80E1, 0x8367);

            texture.loadFromImage(image);

            totalBytes += static_cast<uint64_t>(image.getSize().x * image.getSize().y * 4);
        }

        double elapsed = stopwatch.GetSplit();
        uint64_t byesPerSecond = static_cast<uint64_t>(totalBytes / elapsed);
        cout << FormatMemorySizeFast(byesPerSecond) << " per second" << 
            " (count=" << images.size() << "," << textures.size() <<
            " size=" << FormatMemorySizeFast(totalBytes) << ")" <<
            endl;


/*
        {
            ProcessMemory processMemory;

            cout << "System memory: " << GetMemoryString() << endl;

            cout << "Process memory: " << FormatMemorySizeFast(processMemory.getWorkingSetSize()) <<
                ", Process page usage: " << FormatMemorySizeFast(processMemory.getPagefileUsage()) << endl;
        }

        uint64_t totalSize = 0;
        vector<sf::Texture> textures;

        //for (auto& imageData : images)
        //{
        //    BgraImage image = PavImage::Decompress(imageData);

        //    textures.emplace_back();
        //    textures.back().create(image.getWidth(), image.getHeight());
        //    textures.back().update(image.getData());
        //}
        for (auto& image : images)
        {
            textures.emplace_back();
            textures.back().loadFromImage(image);
        }

        cout << "Western: " << FormatMemorySizeFast(totalSize) << endl;
        {
            ProcessMemory processMemory;

            cout << "System memory: " << GetMemoryString() << endl;

            cout << "Process memory: " << FormatMemorySizeFast(processMemory.getWorkingSetSize()) <<
                ", Process page usage: " << FormatMemorySizeFast(processMemory.getPagefileUsage()) << endl;
        }
*/
/*
        {
            ProcessMemory processMemory;

            cout << "System memory: " << GetMemoryString() << endl;

            cout << "Process memory: " << FormatMemorySizeFast(processMemory.getWorkingSetSize()) <<
                ", Process page usage: " << FormatMemorySizeFast(processMemory.getPagefileUsage()) << endl;
        }

        uint64_t totalSize = 0;
        vector<sf::Image> data;
        vector<sf::Texture> textures;

        for (auto& entry : fs::recursive_directory_iterator("C:/Temp/SuperPick.Western"))
        {
            if (entry.path().extension().generic_wstring() == L".png")
            {
                data.emplace_back();
                data.back().loadFromFile(entry.path().string().c_str());
                uint64_t size = data.back().getSize().x * data.back().getSize().y * sizeof(uint32_t);
                totalSize += size;

                textures.emplace_back();
                textures.back().loadFromImage(data.back());

                ProcessMemory processMemory;
                cout << textures.size() << ',' << processMemory.getWorkingSetSize() << ',' << totalSize << ',' << size << ',' << data.back().getSize().x << ',' << data.back().getSize().y << ',' <<
                    entry.path() << endl;
            }
        }

        cout << "Western: " << FormatMemorySizeFast(totalSize) << endl;
        {
            ProcessMemory processMemory;

            cout << "System memory: " << GetMemoryString() << endl;

            cout << "Process memory: " << FormatMemorySizeFast(processMemory.getWorkingSetSize()) <<
                ", Process page usage: " << FormatMemorySizeFast(processMemory.getPagefileUsage()) << endl;
        }
*/


        //{
        //    ProcessMemory processMemory;

        //    cout << "System memory: " << GetMemoryString() << endl;

        //    cout << "Process memory: " << FormatMemorySizeFast(processMemory.getWorkingSetSize()) <<
        //        ", Process page usage: " << FormatMemorySizeFast(processMemory.getPagefileUsage()) << endl;
        //}

        //uint64_t totalSize = 0;
        //vector<sf::Image> data;

        //for (auto& entry : fs::recursive_directory_iterator("C:/Temp/SuperPick.Western"))
        //{
        //    if (entry.path().extension().generic_wstring() == L".png")
        //    {
        //        data.emplace_back();

        //        WICImaging::ImageData image(entry.path());
        //        data.back().create(image.getWidth(), image.getHeight(), image.getRawPixels());
        //        totalSize += data.back().getSize().x * data.back().getSize().y * sizeof(uint32_t);
        //    }
        //}

        //cout << "Western: " << FormatMemorySizeFast(totalSize) << endl;
        //{
        //    ProcessMemory processMemory;

        //    cout << "System memory: " << GetMemoryString() << endl;

        //    cout << "Process memory: " << FormatMemorySizeFast(processMemory.getWorkingSetSize()) <<
        //        ", Process page usage: " << FormatMemorySizeFast(processMemory.getPagefileUsage()) << endl;
        //}
    }
    OPTIMIZE_ON

    bool PostTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void DeinitTest() override
    {
//#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
//        int x = GL_UNSIGNED_INT_8_8_8_8_REV;
//        int y = GL_UNSIGNED_BYTE;
//
//
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
//
//#define GL_RGBA                           0x1908
//#define GL_RGBA8                          0x8058
//
//#define GL_BGRA_EXT                       0x80E1
    }
    OPTIMIZE_ON
};

static TestAll testAll;
static Test1 test1;
static Test2 test2;
static Test3 test3;
static Test4 test4;

// GL_BGRA with GL_UNSIGNED_INT_8_8_8_8_REV