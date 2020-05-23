#include "stdafx.h"
#include "MakeKeywordQuery.h"

static QChar qvbar       = QChar('|');
static QChar qspace      = QChar(' ');
static QChar qasterix    = QChar('*');
static QChar qquestion   = QChar('?');
static QChar qunderscore = QChar('_');
static QChar qpercent    = QChar('%');

inline
static bool IsPuncOrSpace(const QChar& ch)
{
    return ch.unicode() == 0x007C /* vertical bar*/ || ch.isPunct() || ch.isSpace();
}

inline
static bool IsWildcard(const QChar& ch)
{
    return ch == qasterix || ch == qquestion;
}

inline
static bool IsPuncNotWildcard(const QChar& ch)
{
    return !IsWildcard(ch) && ch.isPunct();
}

inline
static bool IsPuncOrSpaceNotWildcard(const QChar& ch)
{
    return !IsWildcard(ch) && (ch.isPunct() || ch.isSpace());
}

inline
static bool IsWordEnd(const QChar& ch)
{
    ushort uch = ch.unicode();
    return  uch == 0x007C || // vertical bar
            uch == 0x002C || // period
            uch == 0x060C || // arabic period
            uch == 0x3001 || // japanese period
            uch == 0x002E || // comma
            uch == 0X3002 || // japanese comma
        ch.isNull();
}

inline
static bool IsPuncNotWordEnd(const QChar& ch)
{
    return !IsWordEnd(ch) && ch.isPunct();
}

QString SimplifyString(const QString& str, bool prependVBar)
{
    if (str.isEmpty())
    {
        return str;
    }

    QChar* pNewStr = static_cast<QChar*>(stack_alloc((str.size() + 1) * sizeof(ushort)));
    size_t newLen = 0;

    if (prependVBar)
    {
        pNewStr[newLen++] = qvbar;
    }

    size_t emptyLen = newLen;

    for (const QChar* pStr = str.constData(); !pStr->isNull();)
    {
        QChar ch = *pStr++;
        if (ch == qquestion || ch == qvbar)
        {
            continue;
        }

        if (ch.isSpace())
        {
            while (pStr->isSpace() || *pStr == qquestion || *pStr == qvbar)
            {
                pStr++;
            }

            if (pStr->isNull())
            {
                break;
            }

            ch = qspace;
        }

        pNewStr[newLen++] = ch;
    }

    return emptyLen == newLen ? QString() : QString(pNewStr, static_cast<int>(newLen));
}

QString DbSimplifyString2(const QString& str, bool prependVBar)
{
    QString newStr;

    if (!str.isEmpty())
    {
        newStr.reserve(str.size());

        if (prependVBar)
        {
            newStr += qvbar;
        }

        for (auto it = str.begin(); it != str.end(); ++it)
        {
            if (*it == qquestion || *it == qvbar)
            {
                continue;
            }

            if (it->isSpace())
            {
                do
                {
                    ++it;

                } while (it != str.end() && (it->isSpace() || *it == qquestion || *it == qvbar));

                if (it == str.end())
                {
                    break;
                }

                newStr += qspace;
            }
            else
            {
                newStr += *it;
            }
        }
    }

    return newStr;
}

#define MIN_ACCENT	0x00e0
#define MAX_ACCENT	0x00ff

static char stripAccentTable[] =
{
//	's', //	U+00DF 	� 	Latin Small Letter sharp S
    'a', //	U+00E0 	� 	Latin Small Letter A with grave
    'a', //	U+00E1 	� 	Latin Small Letter A with acute
    'a', //	U+00E2 	� 	Latin Small Letter A with circumflex
    'a', //	U+00E3 	� 	Latin Small Letter A with tilde
    'a', //	U+00E4 	� 	Latin Small Letter A with diaeresis
    'a', //	U+00E5 	� 	Latin Small Letter A with ring above
    'a', //	U+00E6 	� 	Latin Small Letter AE
    'c', //	U+00E7 	� 	Latin Small Letter C with cedilla
    'e', //	U+00E8 	� 	Latin Small Letter E with grave
    'e', //	U+00E9 	� 	Latin Small Letter E with acute
    'e', //	U+00EA 	� 	Latin Small Letter E with circumflex
    'e', //	U+00EB 	� 	Latin Small Letter E with diaeresis
    'i', //	U+00EC 	� 	Latin Small Letter I with grave
    'i', //	U+00ED 	� 	Latin Small Letter I with acute
    'i', //	U+00EE 	� 	Latin Small Letter I with circumflex
    'i', //	U+00EF 	� 	Latin Small Letter I with diaeresis
    'd', //	U+00F0 	� 	Latin Small Letter Eth
    'n', //	U+00F1 	� 	Latin Small Letter N with tilde
    'o', //	U+00F2 	� 	Latin Small Letter O with grave
    'o', //	U+00F3 	� 	Latin Small Letter O with acute
    'o', //	U+00F4 	� 	Latin Small Letter O with circumflex
    'o', //	U+00F5 	� 	Latin Small Letter O with tilde
    'o', //	U+00F6 	� 	Latin Small Letter O with diaeresis
    '/', // U+00F7 	� 	Division sign
    'o', //	U+00F8 	� 	Latin Small Letter O with stroke
    'u', //	U+00F9 	� 	Latin Small Letter U with grave
    'u', //	U+00FA 	� 	Latin Small Letter U with acute
    'u', //	U+00FB 	� 	Latin Small Letter U with circumflex
    'u', //	U+00FC 	� 	Latin Small Letter U with diaeresis
    'y', //	U+00FD 	� 	Latin Small Letter Y with acute
    't', //	U+00FE 	� 	Latin Small Letter Thorn
    'y'  //	U+00FF 	� 	Latin Small Letter Y with diaeresis
};

DbMakeKeywordStr::DbMakeKeywordStr(const QString& keywords)
{
    if (keywords.isEmpty())
    {
        _keywordStr = qvbar;
    }
    else
    {
        const QChar* pStr = keywords.constData();
        if (*pStr == qvbar)
        {
            ++pStr;
        }
        else
        {
            _keywordStr = qvbar;
        }

        _keywordStr += keywords;

        QChar cleanTag[MaxTagLen + 1];
        cleanTag[0] = qvbar;
        int cleanTagLen = 1;

        bool firstWord = true;

        for (;;)
        {
            QChar ch = *pStr++;

            if (ch.isLetterOrNumber())
            {
                cleanTag[cleanTagLen++] = ch.unicode() >= MIN_ACCENT && ch.unicode() <= MAX_ACCENT ? stripAccentTable[ch.unicode() - MIN_ACCENT] : ch;
                if (cleanTagLen < MaxTagLen)
                {
                    continue;
                }

                while (!pStr->isNull() && *pStr != qvbar)
                {
                    ++pStr;
                }
            }
            else if (IsPuncNotWordEnd(ch))
            {
                continue;
            }

            if (cleanTagLen > 1)
            {
                QString tag(cleanTag, cleanTagLen);
                if (!_seen.contains(tag))
                {
                    _seen.insert(tag);
                    if (!firstWord)
                    {
                        if (_firstTag)
                        {
                            _keywordStr += qquestion;
                            _firstTag = false;
                        }

                        _keywordStr += tag;
                    }
                }
            }

            if (ch.isNull())
            {
                break;
            }

            if (ch == qvbar)
            {
                firstWord = true;
            }
            else
            {
                while (*pStr != qvbar && (IsWordEnd(*pStr) || IsPuncOrSpace(*pStr)))
                {
                    ++pStr;
                }

                if (*pStr == qvbar)
                {
                    firstWord = true;
                    ++pStr;
                }
                else
                {
                    firstWord = false;
                }
            }

            if (pStr->isNull())
            {
                break;
            }

            cleanTagLen = 1;
        }
    }
}

void DbMakeKeywordStr::Add(const QString& str)
{
    if (!str.isEmpty())
    {
        QString masterStr = str.toLower();
        const QChar* pStr = masterStr.constData();

        QChar cleanTag[MaxTagLen + 1];
        cleanTag[0] = qvbar;
        int cleanTagLen = 1;

        for (;;)
        {
            QChar ch = *pStr++;

            if (ch.isLetterOrNumber())
            {
                cleanTag[cleanTagLen++] = ch.unicode() >= MIN_ACCENT && ch.unicode() <= MAX_ACCENT ? stripAccentTable[ch.unicode() - MIN_ACCENT] : ch;
                if (cleanTagLen < MaxTagLen)
                {
                    continue;
                }

                while (!IsWordEnd(*pStr))
                {
                    ++pStr;
                }
            }
            else if (IsPuncNotWordEnd(ch))
            {
                continue;
            }

            if (cleanTagLen > 1)
            {
                QString tag(cleanTag, cleanTagLen);
                if (!_seen.contains(tag))
                {
                    _seen.insert(tag);

                    if (_firstTag)
                    {
                        _keywordStr += qquestion;
                        _firstTag = false;
                    }

                    _keywordStr += tag;
                }
            }

            if (ch.isNull())
            {
                break;
            }

            while (IsPuncOrSpace(*pStr))
            {
                ++pStr;
            }

            if (pStr->isNull())
            {
                break;
            }

            cleanTagLen = 1;
        }
    }
}

QString MakeKeywordQuery(const QString& keywords)
{
    QString retStr;

    if (!keywords.isEmpty())
    {
        QString keyword = keywords.toLower();

        const QChar* pStr = keyword.constData();
        while (IsPuncOrSpaceNotWildcard(*pStr))
        {
            ++pStr;
        }

        QChar cleanTag[MaxTagLen + 1];
        int cleanTagLen = 0;

        int cleanTagDefaultLen = 1;// preferences.library.useStartsWith ? 1 : 0;
        if (cleanTagDefaultLen)
        {
            cleanTag[cleanTagLen++] = qvbar;
        }

        QSet<QString> seen;
        for (;;)
        {
            QChar ch = *pStr++;

            if (ch == qquestion)
            {
                cleanTag[cleanTagLen++] = qunderscore;
                if (cleanTagLen < MaxTagLen)
                {
                    continue;
                }
            }
            else if (ch == qasterix)
            {
                if (*pStr == qasterix)
                {
                    continue;
                }

                cleanTag[cleanTagLen++] = qpercent;
                if (cleanTagLen < MaxTagLen)
                {
                    continue;
                }
            }
            else if (ch.isLetterOrNumber())
            {
                cleanTag[cleanTagLen++] = ch.unicode() >= MIN_ACCENT && ch.unicode() <= MAX_ACCENT ? stripAccentTable[ch.unicode() - MIN_ACCENT] : ch;
                if (cleanTagLen < MaxTagLen)
                {
                    continue;
                }
            }
            else if (IsPuncNotWordEnd(ch))
            {
                continue;
            }

            if (cleanTagLen)
            {
                if (cleanTag[cleanTagLen - 1] == qpercent)
                {
                    --cleanTagLen;
                }

                if (cleanTagLen)
                {
                    QString tag(cleanTag, cleanTagLen);
                    if (!seen.contains(tag))
                    {
                        seen.insert(tag);

                        if (retStr.isEmpty())
                        {
                            static QString firstLike = QStringLiteral(" keywords LIKE '%");
                            retStr = firstLike;
                        }
                        else
                        {
                            static QString nextLike = QStringLiteral("AND keywords LIKE '%");
                            retStr += nextLike;
                        }

                        static QString closeLike = QStringLiteral("%' ");
                        retStr += tag % closeLike;
                    }
                }
            }

            if (ch.isNull())
            {
                break;
            }

            while (IsPuncOrSpaceNotWildcard(*pStr))
            {
                ++pStr;
            }

            if (pStr->isNull())
            {
                break;
            }

            cleanTagLen = cleanTagDefaultLen;
        }
    }
    return retStr;
}
