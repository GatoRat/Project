// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "WICImage.h"
#include <sstream>
#include "../Error/ErrString.h"
#include "../Error/XceptionT.h"
#include "WICImagingFactory.h"
// The following must be done if in header to prevent tons of warnings:
// ...\shared\rpcndr.h(192): error C2872: 'byte': ambiguous symbol
#define byte win_byte_override
#include <atlbase.h>
#include <wincodec.h>
#include <wincodecsdk.h>
#undef byte

#pragma comment(lib, "Windowscodecs.lib")

using namespace std;
using namespace Graphics;

namespace WICImaging
{
    ImageData::ImageData(const wchar_t* pPathname, const Rect* pRect, uint32_t frameNum)
    {
        load(pPathname, pRect, frameNum);
    }

    ImageData::ImageData(const fs::path& pathname, const Rect* pRect, uint32_t frameNum)
    {
        load(pathname, pRect, frameNum);
    }

    ImageData::ImageData(uint32_t width, uint32_t height, const Pixel* pPixels)
    {
        assign(width, height, pPixels);
    }

    ImageData::ImageData(Size size, const Pixel* pPixels)
    {
        assign(size, pPixels);
    }

    ImageData::ImageData(ImageData&& rhs) noexcept
        : _pixels(move(rhs._pixels))
        , _size(rhs._size)
    {
    }

    ImageData& ImageData::operator=(ImageData&& rhs) noexcept
    {
        if (this != &rhs)
        {
            _pixels = move(rhs._pixels);
            _size = rhs._size;
            rhs._size.width  = 0;
            rhs._size.height = 0;
        }
        return *this;
    }

    void ImageData::clear()
    {
        _pixels.clear();
        _size.width = 0;
        _size.height = 0;
    }

    void ImageData::reset()
    {
        _pixels.resize(0);
        _size.width = 0;
        _size.height = 0;
    }

    bool ImageData::isEmpty() const
    {
        return _size.isEmpty();
    }

    const Pixel* ImageData::operator*() const
    {
        return _pixels.data();
    }

    const uint8_t* ImageData::getPixelPtr() const
    {
        return reinterpret_cast<const uint8_t*>(_pixels.data());
    }

    const vector<Pixel>& ImageData::getPixels() const
    {
        return _pixels;
    }

    const Size& ImageData::getSize() const
    {
        return _size;
    }

    uint32_t ImageData::getWidth() const
    {
        return _size.width;
    }

    uint32_t ImageData::getHeight() const
    {
        return _size.height;
    }

    uint32_t ImageData::getStride() const
    {
        return _size.width * static_cast<uint32_t>(sizeof(Pixel));
    }

    void ImageData::load(const fs::path& pathname, const Rect* pRect, uint32_t frameNum)
    {
        load(pathname.wstring().c_str(), pRect, frameNum);
    }

    void ImageData::load(const wchar_t* pPathname, const Rect* pRect, uint32_t frameNum)
    {
        reset();

        if ((pPathname && *pPathname) && (!pRect || !pRect->isEmpty()))
        {
            try
            {
                IWICImagingFactory* pFactory = GetFactory();

                CComPtr<IWICBitmapDecoder> decoder;
                HRESULT hr = pFactory->CreateDecoderFromFilename(pPathname, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
                if (FAILED(hr))
                {
                    throw XceptionT<HRESULT>(hr, "IWICImagingFactory::CreateDecoderFromFilename()", GetErrStrOS);
                }

                uint32_t numFrames = 0;
                if (SUCCEEDED(decoder->GetFrameCount(&numFrames)))
                {
                    if (frameNum >= numFrames)
                    {
                        frameNum = numFrames - 1;
                    }
                }

                CComPtr<IWICBitmapFrameDecode> frame;
                hr = decoder->GetFrame(frameNum, &frame);
                if (FAILED(hr))
                {
                    stringstream ss;
                    ss << "IWICBitmapDecoder::GetFrame(" << frameNum << ')';
                    throw XceptionT<HRESULT>(hr, ss.str(), GetErrStrOS);
                }

                hr = frame->GetSize(&_size.width, &_size.height);
                if (FAILED(hr))
                {
                    stringstream ss;
                    ss << "IWICBitmapFrameDecode::GetSize(" << frameNum << ')';
                    throw XceptionT<HRESULT>(hr, ss.str(), GetErrStrOS);
                }

                CComPtr<IWICFormatConverter> convertedFrame;
                hr = pFactory->CreateFormatConverter(&convertedFrame);
                if (FAILED(hr))
                {
                    throw XceptionT<HRESULT>(hr, "IWICImagingFactory::CreateFormatConverter()", GetErrStrOS);
                }

                hr = convertedFrame->Initialize(
                        frame,                           // Source frame to convert
                        GUID_WICPixelFormat32bppBGRA,    // The desired pixel format
                        WICBitmapDitherTypeNone,         // The desired dither pattern
                        NULL,                            // The desired palette
                        0.0f,                            // The desired alpha threshold
                        WICBitmapPaletteTypeCustom);     // Palette translation type
                if (FAILED(hr))
                {
                    throw XceptionT<HRESULT>(hr, "IWICFormatConverter::Initialize(GUID_WICPixelFormat32bppBGRA)", GetErrStrOS);
                }

                if (pRect)
                {
                    if (pRect->left + pRect->width > _size.width)
                    {
                        _size.width = _size.width - pRect->left;
                    }
                    else
                    {
                        _size.width = pRect->width;
                    }

                    if (pRect->top + pRect->height > _size.height)
                    {
                        _size.height = _size.height - pRect->top;
                    }
                    else
                    {
                        _size.height = pRect->height;
                    }

                    WICRect srcRect;

                    srcRect.X = static_cast<INT>(pRect->left);
                    srcRect.Y = static_cast<INT>(pRect->top);
                    srcRect.Width = static_cast<INT>(_size.width);
                    srcRect.Height = static_cast<INT>(_size.height);

                    _pixels.resize(_size.width * _size.height); //-V106

                    const uint32_t stride = getStride();
                    const uint32_t imageSize = stride * _size.height;

                    hr = convertedFrame->CopyPixels(&srcRect, stride, imageSize, reinterpret_cast<BYTE*>(&_pixels[0]));
                    if (FAILED(hr))
                    {
                        stringstream ss;
                        ss << "IWICFormatConverter::CopyPixels("
                            "{" << srcRect.X << ',' << srcRect.Y << ',' << srcRect.Width << ',' << srcRect.Height << "}"
                            "," << stride << "," << imageSize << ')';
                        throw XceptionT<HRESULT>(hr, ss.str(), GetErrStrOS);
                    }
                }
                else
                {
                    _pixels.resize(_size.width * _size.height); //-V106

                    const uint32_t stride = getStride();
                    const uint32_t imageSize = stride * _size.height;

                    hr = convertedFrame->CopyPixels(NULL, stride, imageSize, reinterpret_cast<BYTE*>(&_pixels[0]));
                    if (FAILED(hr))
                    {
                        stringstream ss;
                        ss << "IWICFormatConverter::CopyPixels("
                            "NULL"
                            "," << stride << "," << imageSize << ')';
                        throw XceptionT<HRESULT>(hr, ss.str(), GetErrStrOS);
                    }
                }
            }
            catch (XceptionT<HRESULT>&)
            {
                reset();
                throw;
            }
        }
    }

    void ImageData::savePng(const fs::path& pathname)
    {
        savePng(pathname.wstring().c_str());
    }

    void ImageData::savePng(const wchar_t* pPathname)
    {
        //if (!pPathname || !*pPathname)
        //{
        //    throw XceptionT<HRESULT>(WINCODEC_ERR_VALUEOUTOFRANGE);
        //}

        if (_size.isEmpty() || _pixels.empty())
        {
            stringstream ss;
            ss << " _height: " << _size.height <<
                  " _width: " << _size.width <<
                  " _pixels.size(): " << _pixels.size();

            throw XceptionT<HRESULT>(WINCODEC_ERR_BADIMAGE, ss.str(), GetErrStrOS);
        }

        IWICImagingFactory* pFactory = GetFactory();

        CComPtr<IWICBitmapEncoder> encoder;
        HRESULT hr = pFactory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder);
        if (FAILED(hr))
        {
            throw XceptionT<HRESULT>(hr, "IWICImagingFactory::CreateEncoder(GUID_ContainerFormatPng)", GetErrStrOS);
        }

        CComPtr<IWICStream> stream;
        hr = pFactory->CreateStream(&stream);
        if (FAILED(hr))
        {
            throw XceptionT<HRESULT>(hr, "IWICImagingFactory::CreateStream()", GetErrStrOS);
        }

        hr = stream->InitializeFromFilename(pPathname, GENERIC_WRITE);
        if (FAILED(hr))
        {
            throw XceptionT<HRESULT>(hr, "IWICStream::InitializeFromFilename()", GetErrStrOS);
        }

        hr = encoder->Initialize(stream, WICBitmapEncoderNoCache);
        if (FAILED(hr))
        {
            throw XceptionT<HRESULT>(hr, "IWICBitmapEncoder::Initialize(WICBitmapEncoderNoCache)", GetErrStrOS);
        }

        CComPtr<IWICBitmapFrameEncode> frame;
        hr = encoder->CreateNewFrame(&frame, nullptr);
        if (FAILED(hr))
        {
            throw XceptionT<HRESULT>(hr, "IWICBitmapEncoder->CreateNewFrame()", GetErrStrOS);
        }

        hr = frame->Initialize(NULL);
        if (FAILED(hr))
        {
            throw XceptionT<HRESULT>(hr, "IWICBitmapFrameEncode::Initialize()", GetErrStrOS);
        }

        hr = frame->SetSize(_size.width, _size.height);
        if (FAILED(hr))
        {
            stringstream ss;
            ss << "IWICStream::InitializeFromFilename(" << _size.width << ',' << _size.height << ')';
            throw XceptionT<HRESULT>(hr, ss.str(), GetErrStrOS);
        }

        GUID format = GUID_WICPixelFormat32bppBGRA;
        hr = frame->SetPixelFormat(&format);
        if (FAILED(hr))
        {
            throw XceptionT<HRESULT>(hr, "IWICStream::SetPixelFormat(GUID_WICPixelFormat32bppBGRA)", GetErrStrOS);
        }

        if (!IsEqualGUID(format, GUID_WICPixelFormat32bppBGRA))
        {
            throw XceptionT<HRESULT>(hr, "IsEqualGUID(..., GUID_WICPixelFormat32bppBGRA)", GetErrStrOS);
        }

        const uint32_t stride = getStride();
        const uint32_t imageSize = stride * _size.height;

        hr = frame->WritePixels(_size.height, stride, imageSize, reinterpret_cast<BYTE*>(&_pixels[0]));
        if (FAILED(hr))
        {
            stringstream ss;
            ss << "IWICFormatConverter::CopyPixels(" << _size.height << ',' << stride << ',' << imageSize << ')';
            throw XceptionT<HRESULT>(hr, ss.str(), GetErrStrOS);
        }

        hr = frame->Commit();
        if (FAILED(hr))
        {
            throw XceptionT<HRESULT>(hr, "frame->Commit()", GetErrStrOS);
        }

        hr = encoder->Commit();
        if (FAILED(hr))
        {
            throw XceptionT<HRESULT>(hr, "encoder->Commit()", GetErrStrOS);
        }
    }

    Size ImageData::normalizeSize(const Rect& rect) const
    {
        Size size(rect.width, rect.height);

        if (rect.left + rect.width > _size.width)
        {
            size.width = _size.width - rect.left;
        }

        if (rect.top + rect.height > _size.height)
        {
            size.height = _size.height - rect.top;
        }

        return size;
    }

    Rect ImageData::normalizeRect(const Rect& rect) const
    {
        const Size size(normalizeSize(rect));
        return Rect(0, 0, size.width, size.height);
    }

    void ImageData::resize(Graphics::Size size)
    {
        _size = size;
        _pixels.resize(_size.width * _size.height); //-V106
    }

    ImageData& ImageData::copyRect(ImageData& dstImage, const Rect& rect) const
    {
        dstImage.reset();

        if (!rect.isEmpty() && rect.left < _size.width && rect.top < _size.height)
        {
            dstImage.resize(normalizeSize(rect));

            Pixel* pDst = &dstImage._pixels[0];
            const Pixel* pSrc = &_pixels[(_size.width * rect.top) + rect.left]; //-V108
            const size_t dstStride = dstImage.getStride();                      //-V101

            if (dstImage.getWidth() == getWidth())
            {
                memcpy(pDst, pSrc, dstStride * dstImage.getHeight()); //-V104
            }
            else
            {
                for (uint32_t row = 0; row < dstImage.getHeight(); ++row, pSrc += getWidth(), pDst += dstImage.getWidth()) //-V102
                {
                    memcpy(pDst, pSrc, dstStride);
                }
            }
        }

        return dstImage;
    }

    bool ImageData::crop(const Rect& rect)
    {
        bool trimmed = false;

        if (!rect.isEmpty() && rect.left < _size.width && rect.top < _size.height)
        {
            const Size newSize(normalizeSize(rect));

            if (newSize.width < _size.width || newSize.height < _size.height)
            {
                Pixel* pDst = &_pixels[0];
                const Pixel* pSrc = &_pixels[(_size.width * rect.top) + rect.left]; //-V108
                const size_t dstStride = newSize.width * sizeof(Pixel);             //-V104

                if (newSize.width == _size.width)
                {
                    if (pDst != pSrc)
                    {
                        memmove(pDst, pSrc, dstStride * newSize.height); //-V104
                    }
                }
                else
                {
                    for (uint32_t row = 0; row < newSize.height; ++row, pSrc += _size.width, pDst += newSize.width) //-V102
                    {
                        if (pDst != pSrc)
                        {
                            memmove(pDst, pSrc, dstStride);
                        }
                    }
                }

                trimmed = true;
                resize(newSize);
            }
        }

        return trimmed;
    }

    Rect ImageData::getTrimRect() const
    {
        Rect trimRect(0, 0, _size.width, _size.height);

        size_t first = 0;
        for (; first < _pixels.size(); ++first)
        {
            if (_pixels[first].bgra.alpha != 0)
            {
                break;
            }
        }

        if (first < _pixels.size())
        {
            size_t last = _pixels.size();

            for (;;)
            {
                if (_pixels[--last].bgra.alpha != 0)
                {
                    break;
                }
            }

            const size_t topRow = first / _size.width;                       //-V104
            const size_t bottomRow = (last + _size.width - 1) / _size.width; //-V104

            const Pixel* pCurRow = _pixels.data() + (topRow * _size.width);    //-V104
            const Pixel* pEndRow = _pixels.data() + (bottomRow * _size.width); //-V104

            size_t leftMargin = _size.width; //-V101
            size_t rightMargin = 0;

            // Is it that much faster to do the above?

            for (; pCurRow < pEndRow && !(leftMargin == 0 && rightMargin == static_cast<size_t>(_size.width));
                 pCurRow += _size.width) //-V104 //-V102
            {
                for (size_t col = 0; col < _size.width; ++col) //-V104
                {
                    if (pCurRow[col].bgra.alpha != 0)
                    {
                        if (col < leftMargin)
                        {
                            leftMargin = col;
                            break;
                        }
                    }
                }

                for (size_t col = _size.width; col;) //-V101
                {
                    if (pCurRow[--col].bgra.alpha != 0)
                    {
                        if (col >= rightMargin)
                        {
                            rightMargin = col + 1;
                        }
                        break;
                    }
                }
            }

            // we can now calculate the left and width

            trimRect.left   = static_cast<uint32_t>(leftMargin);
            trimRect.width  = static_cast<uint32_t>(rightMargin - leftMargin);
            trimRect.top    = static_cast<uint32_t>(topRow);
            trimRect.height = static_cast<uint32_t>(bottomRow - topRow);
        }

        return trimRect;
    }

    uint64_t ImageData::calcTrimSavings(const Rect& rect) const
    {
        uint64_t savings = rect.width < _size.width ? static_cast<uint64_t>(_size.width - rect.width) * rect.height : 0;
        savings += rect.height < _size.height ? static_cast<uint64_t>(_size.height - rect.height) * _size.width : 0;
        return savings * sizeof(Pixel);
    }

    void ImageData::assign(uint32_t width, uint32_t height, const Pixel* pPixels)
    {
        if (pPixels && width && height)
        {
            _size.width  = width;
            _size.height = height;
            _pixels.assign(pPixels, pPixels + _size.width * _size.height); //-V104
        }
        else
        {
            reset();
        }
    }

    void ImageData::assign(Size size, const Pixel* pPixels)
    {
        assign(size.width, size.height, pPixels);
    }

    void ImageData::set(const Rect& rect_, const Pixel* pSrc)
    {
        if (!pSrc || rect_.isEmpty())
        {
            reset();
        }
        else
        {
            const Rect srcRect(normalizeRect(rect_));

            Pixel* pDst = &_pixels[(_size.width * srcRect.top) + srcRect.left]; //-V108
            const size_t srcStride = sizeof(Pixel) * srcRect.width;             //-V104

            if (srcRect.width == getWidth())
            {
                memcpy(pDst, pSrc, srcStride * srcRect.height); //-V104
            }
            else
            {
                for (uint32_t row = 0; row < srcRect.height; ++row, pDst += getWidth(), pSrc += srcRect.width) //-V102
                {
                    memcpy(pDst, pSrc, srcStride);
                }
            }
        }
    }

    Pixel ImageData::getPixel(uint32_t x, uint32_t y) const
    {
        Pixel pixel;

        if (x < _size.width && y < _size.height)
        {
            pixel = _pixels[(y * _size.width) + x]; //-V108
        }
        else
        {
            pixel.val = 0;
        }

        return pixel;
    }

    Pixel ImageData::getPixel(Position pos) const
    {
        return getPixel(pos.x, pos.y);
    }

    Pixel ImageData::operator[](Graphics::Position pos) const
    {
        return getPixel(pos);
    }

    bool ImageData::clearTransparentPixels()
    {
        bool changed = false;

        for (Pixel& pixel : _pixels)
        {
            if (pixel.bgra.alpha == 0)
            {
                if (pixel.val != 0)
                {
                    pixel.val = 0;
                    changed = true;
                }
            }
        }

        return changed;
    }

    void ImageData::shift(int32_t byX, int32_t /*byY*/)
    {
        if (byX > 0)
        {
            auto rowIt = _pixels.begin();
            for (uint32_t row = 0; row < _size.height; ++row, rowIt += _size.width)
            {
                copy_backward(rowIt, rowIt + _size.width - byX, rowIt + _size.width);
            }
        }
        //else if (byX < 0)
        //{

        //}

        //if (byY > 0)
        //{

        //}
        //else if (byY < 0)
        //{
        //}
    }

} // namespace WICImaging
