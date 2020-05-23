// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>
#include "Graphics.h"

namespace fs = std::filesystem;

namespace WICImaging
{
    class ImageData
    {
    public:
        /** ImageData constructor.
         */
        ImageData() = default;
        ImageData(const wchar_t* pPathname, const Graphics::Rect* pRect = nullptr, uint32_t frameNum = 0);
        ImageData(const fs::path& pathname, const Graphics::Rect* pRect = nullptr, uint32_t frameNum = 0);
        ImageData(uint32_t width, uint32_t height, const Graphics::Pixel* pPixels);
        ImageData(Graphics::Size size, const Graphics::Pixel* pPixels);
        ImageData(ImageData&& rhs) noexcept;
        ImageData& operator=(ImageData&& rhs) noexcept;

        /** ImageData destructor.
         */
        virtual ~ImageData() = default;

        /** Clears the _pixels vector and sets size to zero (0).
         */
        void clear();

        /** Resizes _pixels, and sets size, to zero (0).
         */
        void reset();

        bool isEmpty() const;

        const Graphics::Pixel* operator*() const;
        const uint8_t* getPixelPtr() const;
        const std::vector<Graphics::Pixel>& getPixels() const;
        const Graphics::Size& getSize() const;

        uint32_t getWidth() const;
        uint32_t getHeight() const;
        uint32_t getStride() const;

        Graphics::Pixel getPixel(uint32_t x, uint32_t y) const;
        Graphics::Pixel getPixel(Graphics::Position pos) const;
        Graphics::Pixel operator[](Graphics::Position pos) const;

        /** Loads the specified frame of an image.
           @param pPathname The pathname of the image.
           @param pRect An optional rect of the image to load.
           @param frameNum The frame number.
           @throws XceptionT<HRESULT> on failure.
         */
        void load(const fs::path& pathname, const Graphics::Rect* pRect = nullptr, uint32_t frameNum = 0);
        void load(const wchar_t* pPathname, const Graphics::Rect* pRect = nullptr, uint32_t frameNum = 0);

        /** Saves the data to a PNG file.
           @param pFilename The pathname of the image.
           @throws XceptionT<HRESULT> on failure.
         */
        void savePng(const fs::path& pathname);
        void savePng(const wchar_t* pPathname);

        Graphics::Size normalizeSize(const Graphics::Rect& trimRect) const;
        Graphics::Rect normalizeRect(const Graphics::Rect& trimRect) const;

        ImageData& copyRect(ImageData& dstImage, const Graphics::Rect& rect) const;

        bool crop(const Graphics::Rect& cropRect);

        Graphics::Rect getTrimRect() const;

        uint64_t calcTrimSavings(const Graphics::Rect& rect) const;

        //        bool trim();
        //        bool resize(const Graphics::Rect& rect);

        /** Replaces pixels entirely */
        void assign(uint32_t width, uint32_t height, const Graphics::Pixel* pPixels);
        void assign(Graphics::Size size, const Graphics::Pixel* pPixels);

        /** Replaces pixels in rect */
        void set(const Graphics::Rect& dstRect, const Graphics::Pixel* pPixels);

        bool clearTransparentPixels();

        /** Shifts entire contents; blank pixels will be added to appropriate side.
            \Remarks This is incomplete. It was written to fix up one set of images.
        */
        void shift(int32_t byX, int32_t /*byY*/);

    private:
        void resize(Graphics::Size size);

    private:
        std::vector<Graphics::Pixel> _pixels;
        Graphics::Size _size;
    };
} // namespace WICImaging
