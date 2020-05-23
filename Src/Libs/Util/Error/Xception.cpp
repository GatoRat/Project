// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Xception.h"
#include "ErrString.h"

using namespace std;

string Xception::codeStr() const
{
    auto codeStr = codeStrOpt();
    return codeStr.value_or(GetDefaultErrStr(_code));
}

std::optional<std::string> Xception::codeStrOpt() const
{
    optional<string> codeStr;

    if (_getCodeStr && _code)
    {
        codeStr = _getCodeStr(_code);
    }

    return codeStr;
}

bool Xception::hasWhat() const
{
    return what() && *what();
}

string Xception::errStr(const char* pSuffix, bool /*includePathInfo*/) const
{
    string str(codeStr());

    if (hasWhat())
    {
        if (!str.empty())
        {
            str += ": ";
        }
        str += what();
    }

    if (pSuffix && *pSuffix)
    {
        if (!str.empty())
        {
            str += ": ";
        }
        str += pSuffix;
    }

    return str;
}

void Xception::setPath(const fs::path&/* path*/)
{
}

const fs::path* Xception::getPath() const
{
    return nullptr;
}

ostream& Xception::write(ostream& stream) const
{
    stream << errStr();
    return stream;
}
