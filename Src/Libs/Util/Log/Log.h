// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <string_view>
#include "../Misc/Defines.Misc.h"
#include "../String/ASCII.h"
#include "../System/SysInfo.h"
#include "../Time/TimeStamp.h"

namespace fs = std::filesystem;

namespace Log
{
    constexpr size_t MaxLogSize     = ONE_MEGABYTE;
    constexpr size_t MaxLogSize_Min = ONE_KILOBYTE;
    constexpr size_t MaxLogSize_Max = ONE_MEGABYTE;

    constexpr size_t MaxBackups     = 9;
    constexpr size_t MaxBackups_Min = 0;
    constexpr size_t MaxBackups_Max = 99;

    constexpr size_t MaxDataLen     = 256;
    constexpr size_t MaxDataLen_Min = 16;
    constexpr size_t MaxDataLen_Max = 64 * ONE_KILOBYTE;

    constexpr char   ErrorSymbol    = ASCII::Asterix;
    constexpr char   WarningSymbol  = ASCII::Plus;
    constexpr char   InfoSymbol     = ASCII::Space;
    constexpr char   DebugSymbol    = ASCII::Exclamation;

    /// <summary>
    /// Logs a string.
    /// </summary>
    /// <param name="stringToLog">The string to log.</param>
    void String(std::string_view stringToLog);

    /// <summary>
    /// Logs a string with a timestamp.
    /// </summary>
    /// <param name="timestamp">The timestamp to use.</param>
    /// <param name="stringToLog">The string to log.</param>
    //void String(TimeStamp timestamp, std::string_view stringToLog);

    /// <summary>
    /// Logs a string with a prefix.
    /// </summary>
    /// <param name="prefixToLog">The prefix to log.</param>
    /// <param name="stringToLog">The string to log.</param>
    void String(char prefixToLog, std::string_view stringToLog);

    /// <summary>
    /// Logs two strings.
    /// </summary>
    /// <param name="string1">The first string to log.</param>
    /// <param name="string2">The second string to log, immediately after string1.</param>
    /// <param name="pTimeStamp">An optional pointer to the TimeStamp to use if it's 'valid'.</param>
    /// <remarks>
    /// If both strings are empty, nothing will be logged.
    /// </remarks>
    void Strings(std::string_view string1, std::string_view string2, const TimeStamp* pTimeStamp = nullptr);

    /// <summary>
    /// Formats and logs a string.
    /// </summary>
    /// <param name="formatString">The format string. If empty, nothing will be logged.</param>
    /// <remarks>
    /// Wherever "{0}", "{1}" or "{2}" appears in formatString, an empty string will replace it.
    /// </remarks>
    void Format(std::string_view formatString);

    /// <summary>
    /// Formats and logs a string.
    /// </summary>
    /// <param name="formatString">The format string. If empty, nothing will be logged.</param>
    /// <param name="string0">The string to replace placeholder "{0}".</param>
    /// <remarks>
    /// Wherever "{0}" appears in formatString, string0 will replace it.
    /// Wherever "{1}" or "{2}" appears in formatString, an empty string will replace it.
    /// </remarks>
    void Format(std::string_view formatString, std::string_view string0);

    /// <summary>
    /// Formats and logs a string.
    /// </summary>
    /// <param name="formatString">The format string. If empty, nothing will be logged.</param>
    /// <param name="string0">The string to replace placeholder "{0}".</param>
    /// <param name="string1">The string to replace placeholder "{1}".</param>
    /// <remarks>
    /// All instances of "{0}" and "{1}" will be replaced by their respective string.
    /// Wherever "{2}" appears in formatString, an empty string will replace it.
    /// </remarks>
    void Format(std::string_view formatString, std::string_view string0, std::string_view string1);

    /// <summary>
    /// Formats and logs a string.
    /// </summary>
    /// <param name="formatString">The format string. If empty, nothing will be logged.</param>
    /// <param name="string0">The string to replace placeholder "{0}".</param>
    /// <param name="string1">The string to replace placeholder "{1}".</param>
    /// <param name="string1">The string to replace placeholder "{2}".</param>
    /// <remarks>
    /// All instances of "{0}", "{1}" and "{2}" will be replaced by their respective string.
    /// </remarks>
    void Format(std::string_view formatString, std::string_view string0, std::string_view string1, std::string_view string2);

    /// <summary>
    /// Logs data.
    /// </summary>
    /// <param name="string">The string to prefix to data.</param>
    /// <param name="pData">The data to log.</param>
    /// <param name="dataLength">The length of the data to log, in bytes.</param>
    void Data(std::string_view string, const void* pData, size_t dataLen);

    /// <summary>
    /// Logs system memory usage.
    /// </summary>
    void SystemMemory();

    /// <summary>
    /// Logs process memory usage.
    /// </summary>
    void ProcessMemory();

    /// <summary>
    /// Logs process memory usage.
    /// </summary>
    /// <param name="processMemory">A ProcessMemory instance.</param>
    void ProcessMemory(const ::ProcessMemory& processMemory);

    /// <summary>
    /// Sets limits for the log file.
    /// </summary>
    /// <param name="maxLogSize">The maximum size of a log file.</param>
    /// <param name="maxBackups">The maximum number of backups for a log file.</param>
    void SetLimits(size_t maxLogSize, size_t maxBackups);

    /// <summary>
    /// Sets spam limits for the log file.
    /// </summary>
    /// <param name="timeLimitSeconds">The number of seconds during which an error or warning log entry won't be repeated.</param>
    /// <param name="maxItems">The maximum items to cache. A negative number will reset the cache. INT16_MAX will be ignored.</param>
    /// <remarks>
    /// This does NOT apply to data logging.
    /// </remarks>
    void SetSpamLimits(int16_t timeLimitSeconds, int16_t maxItems = INT16_MAX);

    void SetMaxDataLen(size_t maxDataLen);

    /// <summary>
    /// Sets whether to send logged string to OutputDebugString.
    /// </summary>
    /// <param name="useOutputDebugString">If <b>true</b>, OutputDebugString is
    ///     to be called with the log string contents. This does <i>not</i> apply
    ///     to logged data.</param>
    /// <remarks>
    /// This defaults to <b>true</b> DEBUG builds and <b>false</b> for RELEASE builds.
    /// This is currently _WIN32 only.
    /// </remarks>
    void UseOutputDebugString(bool useOutputDebugString);

    class Listener
    {
    public:
        struct Data
        {
            TimeStamp            timeStamp;
            std::string          str;
            std::vector<uint8_t> data;
        };

        typedef std::function<void(Data& listenerData)> Callback;

    public:
        /// <summary>Listener constructor.</summary>
        /// <param name="listenerCallback">The listener function.</param>
        explicit Listener(Callback listenerCallback);
        Listener(const Listener& other) = delete;
        void operator=(const Listener&) = delete;

        /// <summary>Listener destructor.</summary>
        virtual ~Listener();

        /// <summary>
        /// Stop the listener.
        /// </summary>
        void stop() noexcept;
    };

    /// <summary>
    /// Strips suffix from lambda function name.
    /// </summary>
    /// <param name="pLambdaName">The lambda name retrieved using __FUNCTION__.</param>
    std::string CleanLambdaName(const char* pLambdaName);

    #define CLEAN_LAMBDA_NAME CleanLambdaName(__FUNCTION__)

    /// <summary>
    /// A class to start and stop the logging subsystem.
    /// </summary>
    /// <remarks>
    /// This should be instantiated in main rather than as a global object since
    /// the timing of construction and destruction cannot otherwise be predicted.
    /// </remarks>
    class Init
    {
    public:
        /// <summary>Init constructor.</summary>
        /// <param name="logPath">The path where log file is to be saved.</param>
        /// <param name="header">The header to use when log file is opened.</param>
        /// <param name="maxLogSize">The maximum size of a log file.</param>
        /// <param name="maxBackups">The maximum number of backups for a log file.</param>
        /// <param name="spamTimeout">The number of seconds during which an error or warning log entry won't be repeated.</param>
        /// <param name="spamMaxItems">The maximum items to cache. A negative number will reset the cache. INT16_MAX will be ignored.</param>
        Init(const fs::path& logPath,
            const std::string& header,
            size_t maxLogSize = MaxLogSize,
            size_t maxBackups = MaxBackups,
            int16_t spamTimeout = 0,
            int16_t spamMaxItems = 0);

        /// <summary>Init constructor.</summary>
        /// <param name="maxLogSize">The maximum size of a log file.</param>
        /// <param name="maxBackups">The maximum number of backups for a log file.</param>
        /// <param name="spamTimeout">The number of seconds during which an error or warning log entry won't be repeated.</param>
        /// <param name="spamMaxItems">The maximum items to cache. A negative number will reset the cache. INT16_MAX will be ignored.</param>
        Init(size_t maxLogSize = MaxLogSize,
            size_t maxBackups = MaxBackups,
            int16_t spamTimeout = 0,
            int16_t spamMaxItems = 0);

        static std::string GetDefaultHeader();
        static std::string GetDefaultCloseStr();

        Init(const Init&) = delete;
        void operator=(const Init&) = delete;

        /// <summary>Init destructor.</summary>
        virtual ~Init();
    };
}
