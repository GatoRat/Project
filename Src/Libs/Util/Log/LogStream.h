// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <streambuf>
#include <string>

/*
    cerr.rdbuf(_file.rdbuf());
    sf::err()
*/

class LogStream : public std::basic_streambuf<char>
{
public:
    LogStream(std::ostream& stream, char prefix = '!');
    LogStream(const LogStream&) = delete;
    void operator=(const LogStream&) = delete;
    ~LogStream() override;

protected:
    int_type overflow(int_type v) override;
    std::streamsize xsputn(const char* p, std::streamsize n) override;

private:
    std::ostream&   _stream;
    std::streambuf* _pOrgBuf;
    std::string     _string;
    char            _prefix;
};
