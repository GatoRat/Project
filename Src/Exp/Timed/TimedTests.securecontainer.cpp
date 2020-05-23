// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <cstdint>
#include <string>
#include <vector>
#include "Util/Detect/DetectCompiler.h"
#include "Util/Misc/Defines.Misc.h"
#include "Util/Misc/TimedTest.h"

#include "Util/Secure/SecureContainer.h"
#include "Util/File/FastFile.h"
#include "Util/Misc/MakeFourCC.h"
#include "Util/String/ASCII.h"
#include "Util/Json/JsonWriter.h"
#include "Util/Math/Random.h"
#include <iostream>
#include <sstream>

using namespace std;

#ifdef _MSC_VER
    #pragma warning(disable : 4748) // /GS can not protect parameters and local variables from local buffer overrun because optimizations are disabled in function
#endif

const char* pStart = R"({"messageType":"allBonusRates","sessionId":1164,"gameTypes":[{"id":0,"name":"Super Spin"},{"id":1,"name":"Super Match"},{"id":2,"name":"Super Pick & Win"},{"id":3,"name":"Super Scratch"},{"id":4,"name":"Super Match Pro"},{"id":5,"name":"Super Pick & Win Pro"}],"games":[{"id":2,"name":"Double Dab Coverall Multi-Win"},{"id":5,"name":"Double Dab Coverall Multi-Win"},{"id":6,"name":"Double Dab Coverall Multi-Win"},{"id":7,"name":"Promo Prize"},{"id":8,"name":"Promo Cash"},{"id":9,"name":"Promo Prize"},{"id":10,"name":"Promo Cash"},{"id":13,"name":"Promo"},{"id":14,"name":"Bonanza Multi-Win"},{"id":15,"name":"Coverall Multi-Win"},{"id":16,"name":"Letter X Multi-Win"},{"id":17,"name":"Letter X Multi-Win"},{"id":18,"name":"Lucky 7 Bingo"},{"id":19,"name":"6 Pack Multi-Win"}],)";

vector<string> dataset;
vector<uint8_t> encoded[5];
//const BYTE* pSentData = nullptr;
//UINT  sentLen = 0;

constexpr size_t datasettouse = 0;

//class OptimaPacketTest : public OptimaPacket
//{
//public:
//    OptimaPacketTest()
//    {
//        setKey("");
//
//        setMessageHandler([&](string& message)
//        {
//            lastMessage = move(message);
//        });
//    }
//
//    void verify()
//    {
//        processReceived(encoded[4].data(), encoded[4].size());
//
//        if (dataset[datasettouse] != lastMessage)
//        {
//            cout << "Error: mismatch" << endl;
//        }
//    }
//
//    string lastMessage;
//};
//
//OptimaPacketTest optimaPacket;
//
//BOOL device_send(DEVICE *dev, const BYTE *data, UINT len)
//{
//    dev = dev;
//    vector<uint8_t> enc;
//    enc.resize(len);
//    memcpy(enc.data(), data, len);
//    encoded[4] = move(enc);
//    return TRUE;
//}

void MakeData()
{
    //int count = 2500;
    int count = 250;
    //int count = 4100;

    //for (int count = 100; count <= 2500; count += 2400)
    {
        std::string data{ pStart };

        JsonWriter writer;
        {
            int bonusType = 0;
            int gameId = 1;
            int ballCalls = 99;

            JsonWriter::Array ratesArray("rates", writer);

            for (int rateId = 1; rateId <= count; ++rateId)
            {
                JsonWriter::Object rateObject(writer);

                writer.keyVal("bonusType", bonusType);
                writer.keyVal("gameId", gameId);
                writer.keyVal("rateId", rateId);

                if (rateId % 12 == 0)
                {
                    if (++gameId % 4 == 0)
                    {
                        if (++bonusType == 6)
                        {
                            bonusType = 0;
                        }
                    }
                }

                stringstream ss;

                if (++ballCalls == 100)
                {
                    ballCalls = 38;
                }

                if (ballCalls == 38)
                {
                    ss << "Coverall in " << ballCalls << " or less";
                }
                else if (ballCalls == 50)
                {
                    ballCalls = 99;
                    ss << "Consolation Prize";
                }
                else
                {
                    ss << "Coverall in " << ballCalls << " exactly";
                }

                writer.keyVal("name", ss.str());
                writer.keyVal("ballCalls", ballCalls);
                writer.keyVal("wedgeCount", ballCalls < 99 ? 24 : 12);
            }
        }

        string str = writer.getString();
        data += &str[1];

        dataset.push_back(data);
/*
        {
            SecureContainer::LZ4Compressor lz4Compressor(5);
            encoded[0] = SecureContainer::Encode(data.data(), data.size() + 1, MakeFourCC('P', 'L', 'Z', '4'), nullptr, &lz4Compressor);
        }

        {
            SecureContainer::LZ4Compressor lz4Compressor(1);
            encoded[1] = SecureContainer::Encode(data.data(), data.size() + 1, MakeFourCC('P', 'L', 'Z', '4'), nullptr, &lz4Compressor);
        }

        {
            SecureContainer::LZ4HighCompressor lz4HighCompressor(3);
            encoded[2] = SecureContainer::Encode(data.data(), data.size() + 1, MakeFourCC('P', 'L', 'Z', '4'), nullptr, &lz4HighCompressor);
        }

        {
            SecureContainer::LZ4HighCompressor lz4HighCompressor(10);
            encoded[3] = SecureContainer::Encode(data.data(), data.size() + 1, MakeFourCC('P', 'L', 'Z', '4'), nullptr, &lz4HighCompressor);
        }

        {
            SecureContainer::LZ4HighCompressor lz4HighCompressor(12);
            encoded[4] = SecureContainer::Encode(data.data(), data.size() + 1, MakeFourCC('P', 'L', 'Z', '4'), nullptr, &lz4HighCompressor);
        }
*/
    }
}

struct TestAll : public TimedTest
{
    const char* GetName() override { return "All"; }

    bool InitTest() override
    {
        MakeData();
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
        SecureContainer::LZ4Compressor lz4Compressor(1);
        encoded[0] = SecureContainer::Encode(dataset[datasettouse].data(), dataset[datasettouse].size() + 1, MakeFourCC('P', 'L', 'Z', '4'), nullptr, &lz4Compressor);
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
        SecureContainer::LZ4HighCompressor lz4HighCompressor(10);
        encoded[2] = SecureContainer::Encode(dataset[datasettouse].data(), dataset[datasettouse].size() + 1, MakeFourCC('P', 'L', 'Z', '4'), nullptr, &lz4HighCompressor);
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
        //DEVICE dev;
        //strncpy_s(dev.name, "TestC", sizeof(dev.name));

        //bbSend3(&dev, dataset[datasettouse].data());
    }
    OPTIMIZE_ON

    virtual bool PostTest() override
    {
        return true;
    }

    OPTIMIZE_OFF
    void DeinitTest() override
    {
//        optimaPacket.verify();
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
        //DEVICE dev;
        //strncpy_s(dev.name, "Test", sizeof(dev.name));

        //bbSend2(&dev, dataset[datasettouse].data());

        //HANDLE hheap = HeapCreate(HEAP_NO_SERIALIZE, 0, 0);
        //HeapDestroy(hheap);

        //int32_t LZ4Compressor::calcMaxOutputSize(int32_t dataLen) const
        //{
        //    return LZ4_compressBound(dataLen);
        //}

        //int32_t LZ4Compressor::compress(const void* pSrc, int32_t srcLen, void* pDst, int32_t maxOutputSize) const
        //{
        //    return LZ4_compress_fast(reinterpret_cast<const char*>(pSrc), reinterpret_cast<char*>(pDst), srcLen, maxOutputSize, acceleration);
        //}

        //int32_t LZ4Compressor::Decompress(const void* pSrc, int32_t srcLen, void* pDst, int32_t dstLen)
        //{
        //    return LZ4_decompress_safe(reinterpret_cast<const char*>(pSrc), reinterpret_cast<char*>(pDst), srcLen, dstLen);
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
    }
    OPTIMIZE_ON
};

static TestAll testAll;
static Test1 test1;
static Test2 test2;
static Test3 test3;
static Test4 test4;
