// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Image.h"
#include <algorithm>

using namespace std;

namespace Graphics
{
    Image::Image(uint32_t width, uint32_t height, const Pixel* pPixels)
    {
        set(width, height, pPixels);
    }

    Image::Image(Size size, const Pixel* pPixels)
    {
        set(size, pPixels);
    }

    Image::Image(uint32_t width, uint32_t height, Pixel pixel)
    {
        create(width, height, pixel);
    }

    Image::Image(Size size, Pixel pixel)
    {
        create(size, pixel);
    }

    Image::Image(Image&& rhs) noexcept
        : _pixels(move(rhs._pixels))
        , _size(rhs._size)
    {
    }

    Image& Image::operator=(Image&& rhs) noexcept
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

    void Image::clear()
    {
        _pixels.clear();
        _size.width = 0;
        _size.height = 0;
    }

    void Image::reset()
    {
        _pixels.resize(0);
        _size.width = 0;
        _size.height = 0;
    }

    bool Image::isEmpty() const
    {
        return _size.isEmpty();
    }

    const uint8_t* Image::getData() const
    {
        return reinterpret_cast<const uint8_t*>(_pixels.data());
    }

    uint8_t* Image::getWritableData()
    {
        return reinterpret_cast<uint8_t*>(_pixels.data());
    }

    size_t Image::getDataLen() const
    {
        return _pixels.size() * sizeof(Pixel);
    }

    const Pixel* Image::operator*() const
    {
        return _pixels.data();
    }

    const Pixel* Image::getPixelPtr() const
    {
        return _pixels.data();
    }

    size_t Image::getNumPixels() const
    {
        return _pixels.size();
    }

    const vector<Pixel>& Image::getPixels() const
    {
        return _pixels;
    }

    uint32_t Image::getWidth() const
    {
        return _size.width;
    }

    uint32_t Image::getHeight() const
    {
        return _size.height;
    }

    uint32_t Image::getStride() const
    {
        return _size.width * static_cast<uint32_t>(sizeof(Pixel));
    }

    const Size& Image::getSize() const
    {
        return _size;
    }

    void Image::create(uint32_t width, uint32_t height, Pixel pixel)
    {
        const bool clear = pixel.val == 0 || pixel.bgra.alpha == 0;
        resize(width, height, clear);
        if (!clear)
        {
            fill(pixel);
        }
    }

    void Image::create(Size size, Pixel pixel)
    {
        create(size.width, size.height, pixel);
    }

    void Image::resize(uint32_t width, uint32_t height, bool clear)
    {
        _size.width = width;
        _size.height = height;
        _pixels.resize(_size.width * _size.height); //-V106
        if (clear && !_pixels.empty())
        {
            memset(_pixels.data(), 0, _pixels.size() * sizeof(Pixel));
        }
    }

    void Image::resize(Size size, bool clear)
    {
        resize(size.width, size.height, clear);
    }

    Size Image::normalizeSize(const Rect& rect) const
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

    Rect Image::normalizeRect(const Rect& rect) const
    {
        const Size size(normalizeSize(rect));
        return Rect(0, 0, size.width, size.height);
    }

    Image& Image::copyRect(const Rect& srcRect, Image& dstImage) const
    {
        dstImage.reset();

        if (!srcRect.isEmpty() && srcRect.left < _size.width && srcRect.top < _size.height)
        {
            dstImage.resize(normalizeSize(srcRect), false);

            Pixel* pDst = &dstImage._pixels[0];
            const Pixel* pSrc = &_pixels[(_size.width * srcRect.top) + srcRect.left]; //-V108
            const size_t dstStride = dstImage.getStride();                                  //-V101

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

    bool Image::crop(const Rect& rect)
    {
        bool trimmed = false;

        if (!rect.isEmpty() && rect.left < _size.width && rect.top < _size.height)
        {
            const Size newSize(normalizeSize(rect));

            if (newSize.width < _size.width || newSize.height < _size.height)
            {
                Pixel* pDst = &_pixels[0];
                const Pixel* pSrc = &_pixels[(_size.width * rect.top) + rect.left]; //-V108
                const size_t dstStride = newSize.width * sizeof(Pixel);                   //-V104

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

    Rect Image::getTrimRect() const
    {
        Rect trimRect(0, 0, _size.width, _size.height);

        if (_size.width && _size.height)
        {
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
        }
        return trimRect;
    }

    uint64_t Image::calcTrimSavings(const Rect& rect) const
    {
        uint64_t savings = rect.width < _size.width ? static_cast<uint64_t>(_size.width - rect.width) * rect.height : 0;
        savings += rect.height < _size.height ? static_cast<uint64_t>(_size.height - rect.height) * _size.width : 0;
        return savings * sizeof(Pixel);
    }

    void Image::fill(Pixel pixel)
    {
        std::fill(_pixels.begin(), _pixels.end(), pixel);
    }

    void Image::fill(const Rect& dstRect_, Pixel pixel)
    {
        const Rect dstRect(normalizeRect(dstRect_));

        if (dstRect.left == 0 && dstRect.width == getWidth())
        {
            if (dstRect.top == 0 && dstRect.height == getHeight())
            {
                fill(pixel);
            }
            else
            {
                const auto it = _pixels.begin() + (dstRect.top * getWidth());
                fill_n(it, dstRect.height * getWidth(), pixel);
            }
        }
        else
        {
            auto it = _pixels.begin() + (dstRect.top * getWidth()) + dstRect.left;

            for (uint32_t row = 0; row < dstRect.height; ++row)
            {
                fill_n(it, dstRect.width, pixel);
                it += getWidth();
            }
        }
    }

    void Image::set(uint32_t width, uint32_t height, const Pixel* pPixels)
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

    void Image::set(Size size, const Pixel* pPixels)
    {
        set(size.width, size.height, pPixels);
    }

    void Image::set(const Rect& rect_, const Pixel* pSrc)
    {
        if (!pSrc || rect_.isEmpty())
        {
            reset();
        }
        else
        {
            const Rect srcRect(normalizeRect(rect_));

            Pixel* pDst = &_pixels[(_size.width * srcRect.top) + srcRect.left]; //-V108
            const size_t srcStride = sizeof(Pixel) * srcRect.width;                   //-V104

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

    Pixel Image::getPixel(uint32_t x, uint32_t y) const
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

    Pixel Image::getPixel(Position pos) const
    {
        return getPixel(pos.x, pos.y);
    }

    Pixel Image::operator[](Position pos) const
    {
        return getPixel(pos);
    }

    bool Image::clearTransparentPixels()
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

    void Image::shift(int32_t byX, int32_t /*byY*/)
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
}