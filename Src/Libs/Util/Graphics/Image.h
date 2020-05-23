// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>
#include "Graphics.h"

namespace fs = std::filesystem;

namespace Graphics
{
    class Image
    {
    public:
        /** Image constructor.
        */
        Image() = default;

        Image(uint32_t width, uint32_t height, const Pixel* pPixels);
        Image(Size size, const Pixel* pPixels);
        Image(uint32_t width, uint32_t height, Pixel pixel);
        Image(Size size, Pixel pixel);
        Image(Image&& rhs) noexcept;
        Image& operator=(Image&& rhs) noexcept;

        /** Image destructor.
        */
        virtual ~Image() = default;

        /** Clears the _pixels vector and sets size to zero (0).
        */
        void clear();

        /** Resizes the _pixels vector, and sets size, to zero (0).
        */
        void reset();

        bool isEmpty() const;

        const uint8_t* getData() const;
        size_t         getDataLen() const;
        uint8_t*       getWritableData();

        const Pixel* operator*() const;
        const Pixel* getPixelPtr() const;
        size_t getNumPixels() const;
        const std::vector<Pixel>& getPixels() const;

        uint32_t getWidth() const;
        uint32_t getHeight() const;
        uint32_t getStride() const;

        const Size& getSize() const;

        Pixel getPixel(uint32_t x, uint32_t y) const;
        Pixel getPixel(Position pos) const;
        Pixel operator[](Position pos) const;

        void create(uint32_t width, uint32_t height, Pixel pixel);
        void create(Size size, Pixel pixel);

        void resize(uint32_t width, uint32_t height, bool clear = true);
        void resize(Size size, bool clear = true);

        /** Ensures the returned size is within the image bounds.
        */
        Size normalizeSize(const Rect& trimRect) const;

        /** Ensures the returned rect is within the image bounds.
        */
        Rect normalizeRect(const Rect& trimRect) const;

        Image& copyRect(const Rect& srcRect, Image& dstImage) const;

        bool crop(const Rect& cropRect);

        Rect getTrimRect() const;

        uint64_t calcTrimSavings(const Rect& rect) const;

        //        bool trim();
        //        bool resize(const Rect& rect);

        /** Replaces pixels entirely */
        void fill(Pixel pixel);

        void fill(const Rect& dstRect, Pixel pixel);

        /** Replaces pixels entirely */
        void set(uint32_t width, uint32_t height, const Pixel* pPixels);
        void set(Size size, const Pixel* pPixels);

        /** Replaces pixels in rect */
        void set(const Rect& dstRect, const Pixel* pPixels);

        bool clearTransparentPixels();

        /** Shifts entire contents; blank pixels will be added to appropriate side.
            \Remarks This is incomplete. It was written to fix up one set of images.
        */
        void shift(int32_t byX, int32_t /*byY*/);

    private:
        std::vector<Pixel> _pixels;
        Size _size;
    };
}