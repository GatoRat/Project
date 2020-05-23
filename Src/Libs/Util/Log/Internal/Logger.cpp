// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Logger.h"
#include "../../File/RenameBackupFiles.h"
#include "../../String/ASCII.h"
#include "../../String/Convert.h"
#include "../../String/Str.h"
#include "../../System/SysInfo.h"
#include "../../System/_lean_windows_.h"
#include "../../Time/DateTime.h"

using namespace std;

namespace Log::Internal
{
    Logger::Logger(const fs::path& pathname, const string& header, size_t maxLogSize, size_t maxBackups)
        : _pathname(pathname)
        , _header(header)
#ifdef _DEBUG
        , _useOutputDebugString(true)
#else
        , _useOutputDebugString(false)
#endif
    {
        setLimits(maxLogSize, maxBackups);

        if (Str::Compare(_pathname.extension().c_str(), L".log") == 0)
        {
            _pathname.replace_extension(L".log");
        }

        open();
    }

    void Logger::setLimits(size_t maxLogSize, size_t maxBackups)
    {
        _maxLogSize = maxLogSize;
        _maxBackups = maxBackups;
    }

    void Logger::setSpamLimits(int16_t timeLimitSeconds, int16_t maxItems)
    {
        scoped_lock<mutex> lock(_spamLimiterMutex);
        if (_spamLimiter)
        {
            if (maxItems == 0)
            {
                _spamLimiter.reset();
            }
            else
            {
                _spamLimiter->setLimits(timeLimitSeconds, maxItems);
            }
        }
        else if (timeLimitSeconds > 0 && maxItems > 0)
        {
            _spamLimiter = make_shared<SpamLimiter>(timeLimitSeconds, maxItems);
        }
    }

    void Logger::useOutputDebugString(bool useOutputDebugString)
    {
        _useOutputDebugString = useOutputDebugString;
    }

    void Logger::setListenerCallback(Log::Listener::Callback listenerCallback)
    {
        _listenerCallback = move(listenerCallback);
    }

    void Logger::log(string_view stringToLog)
    {
        log(stringToLog, "");
    }

    void Logger::log(string_view stringToLog1, string_view stringToLog2, const TimeStamp* pTimeStamp)
    {
        Log::Listener::Data listenerData;

        string logStr;
        logStr.reserve(DateTime::FastAppendMaxSize + 1 + stringToLog1.size() + stringToLog2.size() + 1);

        if (pTimeStamp && pTimeStamp->validate())
        {
            listenerData.timeStamp = *pTimeStamp;
        }
        else
        {
            listenerData.timeStamp.setCurTime();
        }

        listenerData.timeStamp.toStrAppend(logStr, TimeStamp::UseMilliseconds);

        logStr += ASCII::Space;

        const size_t prefixLen = logStr.size();

        if (Str::StartsWith(stringToLog1, "Error:"))
        {
            logStr += ErrorSymbol;
        }
        else if (Str::StartsWith(stringToLog1, "Warning:"))
        {
            Str::Front(stringToLog1) == ASCII::Space ? stringToLog1.remove_prefix(9) : stringToLog1.remove_prefix(8);
            logStr += WarningSymbol;
        }
        else if (Str::StartsWith(stringToLog1, "Info:"))
        {
            Str::Front(stringToLog1) == ASCII::Space ? stringToLog1.remove_prefix(6) : stringToLog1.remove_prefix(5);
            logStr += InfoSymbol;
        }
        else if (Str::StartsWith(stringToLog1, "Debug:"))
        {
            Str::Front(stringToLog1) == ASCII::Space ? stringToLog1.remove_prefix(7) : stringToLog1.remove_prefix(6);
            logStr += DebugSymbol;
        }
        else
        {
            const char ch = stringToLog1.empty() ? ASCII::NUL : stringToLog1.front();
            if (ch != ErrorSymbol && ch != WarningSymbol && ch != InfoSymbol && ch != DebugSymbol)
            {
                logStr += ASCII::Space;
            }
        }

        logStr += stringToLog1;
        logStr += stringToLog2;
        logStr += ASCII::LF;

        bool doWrite = true;
        if (logStr[prefixLen] == ErrorSymbol || logStr[prefixLen] == WarningSymbol)
        {
            scoped_lock<mutex> lock(_spamLimiterMutex);
            if (_spamLimiter)
            {
                string_view subStr{ logStr };
                subStr.remove_prefix(prefixLen);
                doWrite = _spamLimiter->check(subStr);
            }
        }

        if (doWrite)
        {
            if (_useOutputDebugString)
            {
                scoped_lock<recursive_mutex> lock(_mutex);
                OutputDebugStringA(logStr.c_str());
            }

            if (_listenerCallback)
            {
                listenerData.str = logStr.substr(prefixLen, logStr.size() - prefixLen - 1);
                _listenerCallback(listenerData);
            }

            write(logStr);
        }
    }

    void Logger::logFormat(string_view formatString, string_view stringToLog0, string_view stringToLog1, string_view stringToLog2)
    {
        string str;
        str.reserve(formatString.size() + stringToLog0.size() + stringToLog1.size() + stringToLog2.size());
        // The reserve is optimistic.

        str = formatString;
        Str::ReplaceAll(str, "{0}", stringToLog0);
        Str::ReplaceAll(str, "{1}", stringToLog1);
        Str::ReplaceAll(str, "{2}", stringToLog2);

        log(str, "");
    }

    //void Logger::log(string_view stringToLog, int64_t valueToLog)
    //{
    //    log(stringToLog, to_string(valueToLog));
    //}

    constexpr size_t BytesPerLine = 16;
    constexpr size_t OffsetLen    = 4;
    // YYYY-MM-DD HH:MM::SS.mmm Data: 4,096 bytes\n
    constexpr size_t HeaderLen = DateTime::FastAppendMaxSize + 1 + 6 + 5 + 5 + 1;
    //   XXXX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX ................\n
    constexpr size_t DataLineLen = 2 + OffsetLen + (BytesPerLine * 3) + 1 + BytesPerLine + 1;

    void Logger::log(string_view stringToLog, const void* pData_, size_t dataLen)
    {
        const size_t fullRows  = dataLen / BytesPerLine;
        const size_t remainder = dataLen % BytesPerLine;

        string str;
        str.reserve(HeaderLen + (DataLineLen * (fullRows + 1)) + stringToLog.size());

        Log::Listener::Data listenerData;
        listenerData.timeStamp.setCurTime();
        listenerData.timeStamp.toStrAppend(str, TimeStamp::UseMilliseconds);

        if (_listenerCallback)
        {
            listenerData.str = stringToLog;
        }

        str += stringToLog;
        str += " #Data: ";
        Convert::ToStr64(str, dataLen, 0, ASCII::Space, ASCII::Comma);

        if (pData_)
        {
            str += " bytes\n";

            auto pData = static_cast<const char*>(pData_);

            if (_listenerCallback)
            {
                listenerData.data.assign(pData, pData + dataLen);
            }

            size_t row = 0;
            for (; row < fullRows; ++row)
            {
                str.append(2, ASCII::Space);
                Convert::AppendHex(str, (row * BytesPerLine), false, 4); //-V112
                str += ASCII::Space;
                Convert::AppendHex(str, pData, BytesPerLine, false, ASCII::Space);
                str += ASCII::Space;
                for (size_t col = 0; col < BytesPerLine; ++col)
                {
                    const char ch = *pData++;
                    str += ASCII::IsPrintable(ch) ? ch : ASCII::Period;
                }
                str += ASCII::LF;
            }

            if (remainder)
            {
                str.append(2, ASCII::Space);
                Convert::AppendHex(str, (row * BytesPerLine), false, 4); //-V112
                str += ASCII::Space;
                Convert::AppendHex(str, pData, remainder, false, ASCII::Space);
                str.append((BytesPerLine - remainder) * 3 + 1, ASCII::Space);

                for (size_t col = 0; col < remainder; ++col)
                {
                    const char ch = *pData++;
                    str += ASCII::IsPrintable(ch) ? ch : ASCII::Period;
                }
                str += ASCII::LF;
            }
        }
        else
        {
            str += " bytes (pData==nullptr)\n";
        }

        write(str);

        if (_listenerCallback)
        {
            _listenerCallback(listenerData);
        }
    }

    void Logger::write(string_view str)
    {
        scoped_lock<recursive_mutex> lock(_mutex);

        if (static_cast<size_t>(_file.tellp()) > _maxLogSize)
        {
            _file.close();

            if (_maxBackups == 0)
            {
                remove(_pathname);
            }
            else
            {
                RenameBackupFiles(_pathname, static_cast<uint16_t>(_maxBackups));
            }

            open();
        }

        _file << str;
        _file.flush();
    }

    // Call only from constructor or write()
    void Logger::open()
    {
        _file.open(_pathname, ios_base::out | ios_base::app);

        if (!_header.empty())
        {
            logFormat(_header, GetMemoryString() , "", "");
        }
    }

} // namespace Log::Internal
