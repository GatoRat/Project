// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <atomic>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>
#include <string_view>
#include "../Log.h"
#include "SpamLimiter.h"

namespace fs = std::filesystem;

namespace Log::Internal
{
    class Logger final
    {
    public:
        Logger(const fs::path& pathname, const std::string& header, size_t maxLogSize, size_t maxBackups);
        Logger(const Logger&) = delete;
        void operator=(const Logger&) = delete;
        ~Logger() = default;

        /// <summary>
        /// Sets limits for the log file.
        /// </summary>
        /// <param name="maxLogSize">The maximum size of a log file.</param>
        /// <param name="maxBackups">The maximum number of backups for a log file.</param>
        void setLimits(size_t maxLogSize, size_t maxBackups);

        /// <summary>
        /// Sets spam limits for the log file.
        /// </summary>
        /// <param name="timeLimitSeconds">The number of seconds during which an error or warning log entry won't be repeated.</param>
        /// <param name="maxItems">The maximum items to cache. A negative number will reset the cache.</param>
        /// <remarks>
        /// This does NOT apply to data logging.
        /// </remarks>
        void setSpamLimits(int16_t timeLimitSeconds, int16_t maxItems = 256);

        void useOutputDebugString(bool useOutputDebugString);

        /// <summary>
        /// Sets a listener function to call for every item logged.
        /// </summary>
        /// <param name="listenerCallback">The function to call for every item logged or nullptr to stop listening.</param>
        /// <remarks>
        /// There can be only one listener. Currently, there is no use case for more than one.
        /// </remarks>
        void setListenerCallback(Log::Listener::Callback listenerCallback);

        void log(std::string_view stringToLog);
        void log(std::string_view stringToLog1, std::string_view stringToLog2, const TimeStamp* pTimeStamp = nullptr);

        void logFormat(std::string_view formatString, std::string_view stringToLog0, std::string_view stringToLog1, std::string_view stringToLog2);

        void log(std::string_view stringToLog, const void* pData, size_t dataLen);

    private:
        void open();
        void write(std::string_view str);

    private:
        fs::path             _pathname;
        std::ofstream        _file;
        std::string          _header;
        std::recursive_mutex _mutex;
        std::atomic<size_t>  _maxLogSize = Log::MaxLogSize;
        std::atomic<size_t>  _maxBackups = Log::MaxBackups;
        std::atomic<bool>    _useOutputDebugString;
        std::mutex           _spamLimiterMutex;
        std::shared_ptr<SpamLimiter> _spamLimiter;
        Log::Listener::Callback      _listenerCallback;
    };
} // namespace Log::Internal
