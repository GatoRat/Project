// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Log.h"
#include <memory>
#include <mutex>
#include <sstream>
#include "../Misc/AppInfo.h"
#include "../String/ASCII.h"
#include "../String/FormatSizeFast.h"
#include "../System/SysGetPath.h"
#include "Internal/Logger.h"

using namespace std;

namespace Log
{
    static shared_ptr<Internal::Logger> logger;
    static mutex          loggerCreateMutex;
    static string_view    emptyStr;
    static size_t         maxDataLen = MaxDataLen;

    static shared_ptr<Internal::Logger> GetLogger()
    {
        unique_lock<mutex> lock(loggerCreateMutex);
        shared_ptr<Internal::Logger> tmpLogger(logger);
        return tmpLogger;
    }

    void String(string_view stringToLog)
    {
        auto tmpLogger{ GetLogger() };
        if (tmpLogger && !stringToLog.empty())
        {
            tmpLogger->log(stringToLog);
        }
    }

    void String(char prefixToLog, string_view stringToLog)
    {
        auto tmpLogger{ GetLogger() };
        if (tmpLogger && !stringToLog.empty())
        {
            if (ASCII::IsPrintable(prefixToLog))
            {
                tmpLogger->log(&prefixToLog, stringToLog);
            }
            else
            {
                tmpLogger->log(stringToLog);
            }
        }
    }

    void Strings(string_view string1, string_view string2, const TimeStamp* pTimeStamp)
    {
        auto tmpLogger{ GetLogger() };
        if (tmpLogger && !(string1.empty() && string2.empty()))
        {
            tmpLogger->log(string1, string2, pTimeStamp);
        }
    }

    void Format(string_view formatString)
    {
        Format(formatString, emptyStr, emptyStr, emptyStr);
    }

    void Format(string_view formatString, string_view string0)
    {
        Format(formatString, string0, emptyStr, emptyStr);
    }

    void Format(string_view formatString, string_view string0, string_view string1)
    {
        Format(formatString, string0, string1, emptyStr);
    }

    void Format(string_view formatString, string_view string0, string_view string1, string_view string2)
    {
        auto tmpLogger{ GetLogger() };
        if (tmpLogger && !formatString.empty())
        {
            tmpLogger->logFormat(formatString, string0, string1, string2);
        }
    }

    void Data(string_view string, const void* pData, size_t dataLen)
    {
        unique_lock<mutex> lock(loggerCreateMutex);
        shared_ptr<Internal::Logger> tmpLogger(logger);

        if (dataLen > maxDataLen)
        {
            dataLen = maxDataLen;
        }

        lock.release();

        tmpLogger->log(string, pData, dataLen);
    }

    void SystemMemory()
    {
        Log::Format("System memory: {0}", GetMemoryString());
    }

    void ProcessMemory(const ::ProcessMemory& processMemory)
    {
        Log::Format("Process memory: {0}, Process page usage: {1}",
            FormatMemorySizeFast(processMemory.getWorkingSetSize()),
            FormatMemorySizeFast(processMemory.getPagefileUsage()));
    }

    void ProcessMemory()
    {
        const ::ProcessMemory processMemory;
        ProcessMemory(processMemory);
    }

    void SetLimits(size_t maxLogSize, size_t maxBackups)
    {
        auto tmpLogger{ GetLogger() };
        if (tmpLogger)
        {
            if (maxLogSize < MaxLogSize_Min)
            {
                maxLogSize = MaxLogSize_Min;
            }
            else if (maxLogSize > MaxLogSize_Max)
            {
                maxLogSize = MaxLogSize_Max;
            }

            if (maxBackups > MaxBackups_Max)
            {
                maxBackups = MaxBackups_Max;
            }

            tmpLogger->setLimits(maxLogSize, maxBackups);
        }
    }

    void SetSpamLimits(int16_t timeLimitSeconds, int16_t maxItems)
    {
        auto tmpLogger{ GetLogger() };
        if (tmpLogger)
        {
            tmpLogger->setSpamLimits(timeLimitSeconds, maxItems);
        }
    }

    void SetMaxDataLen(size_t maxDataLen_)
    {
        if (maxDataLen_ < MaxDataLen_Min)
        {
            maxDataLen = MaxDataLen_Min;
        }
        else if (maxDataLen_ > MaxDataLen_Max)
        {
            maxDataLen = MaxDataLen_Max;
        }
        else
        {
            maxDataLen = maxDataLen_;
        }
    }

    void UseOutputDebugString(bool useOutputDebugString)
    {
        auto tmpLogger{ GetLogger() };
        if (tmpLogger)
        {
            tmpLogger->useOutputDebugString(useOutputDebugString);
        }
    }

    static void setListenerCallback(Listener::Callback listenerCallback)
    {
        auto tmpLogger{ GetLogger() };
        if (tmpLogger)
        {
            tmpLogger->setListenerCallback(listenerCallback);
        }
    }

    Listener::Listener(Callback listenerCallback)
    {
        setListenerCallback(listenerCallback);
    }

    Listener::~Listener()
    {
        stop();
    }

    void Listener::stop() noexcept
    {
        setListenerCallback(nullptr);
    }

    //2018/02/13 12:51:58.497 *Error : Parsing message : Member not found : patterns : [Application::setMessageHandler::<lambda_6d7e53b382d619e5d15f8eea79bb44d6>::operator (), 318]

    string CleanLambdaName(const char* pLambdaName)
    {
        string lambdaName(pLambdaName ? pLambdaName : "");
        const auto pos = lambdaName.find("::<lambda", 0);
        if (pos != string::npos)
        {
            lambdaName.resize(pos);
            lambdaName += "::<lambda...>()";
        }
        return lambdaName;
    }

    Init::Init(const fs::path& logPath, const string& header, size_t maxLogSize, size_t maxBackups, int16_t spamTimeout, int16_t spamMaxItems)
    {
        unique_lock<mutex> lock(loggerCreateMutex);
        if (!logger)
        {
            logger = make_shared<Internal::Logger>(logPath, header, maxLogSize, maxBackups);
            logger->setSpamLimits(spamTimeout, spamMaxItems);
        }
    }

    string Init::GetDefaultHeader()
    {
        stringstream ss;
        ss << "### " << AppInfo::GetName() << " " << AppInfo::GetVersionStr() << " (" <<
#ifdef _WIN64
            "64"
#else
            "32"
#endif
            "-bit) | " << GetOsString() << " | " << GetProcessorString() << " | {0} ###";
        return ss.str();
    }

    string Init::GetDefaultCloseStr()
    {
        stringstream ss;
        ss << "< Close | " << GetMemoryString() << " >";
        return ss.str();
    }

    Init::Init(size_t maxLogSize, size_t maxBackups, int16_t spamTimeout, int16_t spamMaxItems)
        : Init(SysGetPath::AppData() / "error.log", GetDefaultHeader(), maxLogSize, maxBackups, spamTimeout, spamMaxItems)
    {
    }

    Init::~Init()
    {
        unique_lock<mutex> lock(loggerCreateMutex);
        logger.reset();
    }
} // namespace Log
