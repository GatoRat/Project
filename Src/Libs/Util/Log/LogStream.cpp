// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "LogStream.h"
#include <Util/String/ASCII.h>
#include "Log.h"

using namespace std;

LogStream::LogStream(ostream& stream, char prefix)
    : _stream(stream)
    , _prefix(prefix)
{
    _pOrgBuf = stream.rdbuf();
    stream.rdbuf(this);
}

LogStream::~LogStream()
{
    // output anything that is left
    if (!_string.empty())
    {
        Log::String(_string);
    }

    try
    {
        _stream.rdbuf(_pOrgBuf);
    }
    catch (const ios_base::failure&) // swallow exception
    {
    }
}

streambuf::int_type LogStream::overflow(int_type v)
{
    if (v == ASCII::LF)
    {
        Log::String(_string);
        _string.erase(_string.begin(), _string.end());
    }
    else
    {
        _string += static_cast<char>(v);
    }
    return v;
}

streamsize LogStream::xsputn(const char* p, streamsize n)
{
    _string.append(p, p + n);

    for (size_t pos = 0; pos != string::npos;)
    {
        pos = _string.find(ASCII::LF);
        if (pos != string::npos)
        {
            string tmpStr(_string.begin(), _string.begin() + pos);
            Log::String(_prefix, tmpStr);

            _string.erase(_string.begin(), _string.begin() + pos + 1);
        }
    }

    return n;
}
