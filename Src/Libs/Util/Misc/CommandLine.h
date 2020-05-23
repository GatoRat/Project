// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <optional>
#include <string>
#include <vector>
#include "../Error/XceptionT.h"

namespace CommandLine
{
    enum class Options : uint32_t
    {
        AllowHelpArg    = 0b00000000,    // Help is always available
        AllowPauseArg   = 0b00000001,
        AllowVerboseArg = 0b00000010,
        // Reserved     = 0b00000100,
        // Reserved     = 0b00001000,
        AllowAllArgs    = 0b00001111,

        UsageOnNoArgs   = 0b00010000,
        UserOptions     = 0xff00
    };

    constexpr Options operator|(Options a, Options b)
    {
        return static_cast<Options>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    constexpr uint32_t operator&(Options a, Options b)
    {
        return static_cast<uint32_t>(a) & static_cast<uint32_t>(b);
    }

    enum Error
    {
        Success = 0,
        BadSwitch,
        BadArg,
        InvalidArg,
    };

    enum class UsageType
    {
        Args,
        Detail,
        Extra
    };

    constexpr bool IsDashOrSlash(char ch)
    {
        return ch == '-' || ch == '/';
    }

    constexpr bool IsDashOrSlash(wchar_t ch)
    {
        return ch == L'-' || ch == L'/';
    }

    /** UTF-8 Command Line Arguments class
    */
    class Args
    {
    public:
        Args() = default;
        virtual ~Args() = default;

        void parse(int argc, char* argv[]);

    protected:
        virtual void parseArg(unsigned argNum, const char* pArg);
        virtual const char* getUsage(UsageType usageType);
        virtual bool verify();

        void printUsage();

    public:
        std::vector<std::string> args;
    };

    /** UTF-16 Command Line Arguments class
    */
    class ArgsW
    {
    public:
        ArgsW() = default;
        virtual ~ArgsW() = default;

        void parse(int argc, wchar_t* argv[], Options options = Options::AllowAllArgs);

    protected:
        virtual void parseArg(unsigned argNum, const wchar_t* pArg);
        virtual const wchar_t* getUsageArgs() const;
        virtual const wchar_t* getUsageDetail() const;
        virtual const wchar_t* getUsageExtra() const;
        virtual bool verify();

        void printUsage() const;

    protected:
        Options _options;

    public:
        std::vector<std::wstring> _args;
    };

    /** Writes the program, version and copyright notice to cout
    */
    void WriteNotice(const char* pCopyright);

} // namespace CommandLine

/** Indicates whether verbose output should be done.
    \returns \b true, if verbose output should be done, else \b false.
*/
bool VerboseOutput();

int ShowExitCode(int exitCode, bool showExitCode = true);
