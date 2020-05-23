#pragma once

constexpr size_t MaxTagLen = 40; // NOT including terminating zero

QString SimplifyString(const QString& str, bool prependVBar = false);
QString MakeKeywordQuery(const QString& keywords);

struct DbMakeKeywordStr
{
    QSet<QString> _seen;
    QString		  _keywordStr;
    bool		  _firstTag = true;

    // Pass the primary pipe delimeted keyword string. Must already be simplified and lower case: only alphanumeric, no leading/trailing whitepace and single spaces
    DbMakeKeywordStr(const QString& keywords);
    DbMakeKeywordStr(const DbMakeKeywordStr&) = delete;
    void operator=(const DbMakeKeywordStr&) = delete;

    // Must already be simplified: only alphanumeric, no leading/trailing whitepace and single spaces. It will be converted to lower case.
    void Add(const QString& str);
};