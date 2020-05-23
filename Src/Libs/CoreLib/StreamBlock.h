// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

struct StreamBlock
{
public:
    QDataStream& stream;

protected:
    qint64 startPos;
    qint32 length;

public:
    StreamBlock(QDataStream& stream)
        : stream(stream)
    {
        SetStart();
        stream >> length;
        if (length < 0)
            length = -length;
    }

    StreamBlock(QDataStream& stream, qint32 length)
        : stream(stream)
        , length(length >= 0 ? length : -length)
    {
        SetStart();
    }

    ~StreamBlock()
    {
        if (length)
            stream.device()->seek(startPos + length);
    }

    qint64 GetStartPos() const      { return startPos; }
    qint32 GetLength() const        { return length; }
    bool   HasLength() const        { return length != 0; }
    bool   IsPast() const           { return stream.device()->pos() >= startPos + length; }
    qint64 Pos() const              { return stream.device()->pos(); }
    bool   AtEnd() const            { return stream.atEnd(); }
    QDataStream::Status Status() const
                                    { return stream.status(); }
    QDataStream& Stream() const     { return stream; }
    operator QDataStream&() const   { return stream; }

    void   SetStart()               { startPos = stream.device()->pos(); }
    void   Clear()                  { length = 0; }

    bool CanRead(int valLen) const  { return stream.device()->pos() + valLen <= startPos + length; }

    int ReadRawData(void* pBuffer, int len) { return stream.readRawData((char*)pBuffer, len); }

    _CORELIB_DISALLOW_COPY_AND_ASSIGN(StreamBlock);
};

#define StreamBlockCanRead(valLen) \
        if (streamBlock.AtEnd())    throw FileException(streamBlock, ErrCode::ReadEndOfFile, _WHERE_); \
        if (!streamBlock.CanRead(valLen)) throw FileException(streamBlock, ErrCode::ReadEndOfBlock, _WHERE_)

#define StreamBlockCheckEof() \
        if (streamBlock.Status() == QDataStream::ReadPastEnd) throw FileException(streamBlock, ErrCode::ReadEndOfFile, _WHERE_)

#define StreamBlockRead(val)                    StreamBlockCanRead(sizeof(val)); (QDataStream&)streamBlock >> val; StreamBlockCheckEof()

#define StreamBlockReadRawData(pBuffer, len)    StreamBlockCanRead(len); streamBlock.ReadRawData(pBuffer, len); StreamBlockCheckEof()

#define StreamBlockCanRead2(streamBlock, valLen) \
        if (streamBlock.AtEnd())    throw FileException(streamBlock, ErrCode::ReadEndOfFile, _WHERE_); \
        if (!streamBlock.CanRead(valLen)) throw FileException(streamBlock, ErrCode::ReadEndOfBlock, _WHERE_)

#define StreamBlockCheckEof2(streamBlock) \
        if (streamBlock.stream.status() == QDataStream::ReadPastEnd) throw FileException(streamBlock, ErrCode::ReadEndOfFile, _WHERE_)

#define StreamBlockRead2(streamBlock, val)                  StreamBlockCanRead2(streamBlock, sizeof(val)); (QDataStream&)streamBlock >> val; StreamBlockCheckEof2(streamBlock)

#define StreamBlockReadRawData2(streamBlock, pBuffer, len)  StreamBlockCanRead2(streamBlock, len); streamBlock.ReadRawData(pBuffer, len); StreamBlockCheckEof2(streamBlock)
