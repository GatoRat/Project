// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Util/Graphics/WICImage.h"
#include "TestHelper/catch.hpp"

using namespace std;
using namespace Graphics;
using namespace WICImaging;

TEST_CASE("WICImage")
{
    SECTION("ctor")
    {
        ImageData imageData;

        REQUIRE(imageData.isEmpty() != false); //-V521
        REQUIRE(imageData.getPixels().size() == 0); //-V521
        REQUIRE(imageData.getWidth() == 0); //-V521
        REQUIRE(imageData.getHeight() == 0); //-V521
        REQUIRE(imageData.getStride() == 0); //-V521
    }
    SECTION("assignment")
    {
        Size size(2, 3);
        Pixel orgPixels[6] =
        {
            0x11223344, 0x22334455, 0x33445566, 0x44556677, 0x55667788, 0x66778899
        };

        SECTION("ctor")
        {
            ImageData imageData(size, orgPixels);

            REQUIRE(imageData.isEmpty() == false); //-V521
            REQUIRE(imageData.getWidth() == 2); //-V521
            REQUIRE(imageData.getHeight() == 3); //-V521

            auto& pixels = imageData.getPixels();
            REQUIRE(pixels.size() == 6); //-V521

            for (size_t i = 0; i < pixels.size(); ++i)
            {
                REQUIRE(orgPixels[i].val == pixels[i].val); //-V521
            }
        }
        SECTION("assign()")
        {
            ImageData imageData(size, orgPixels);

            Size newSize(1, 2);
            Pixel newPixels[2] =
            {
                0x778899aa, 0x8899aabb
            };

            SECTION("assign(empty, nullptr)")
            {
                imageData.assign(Size(), nullptr);

                REQUIRE(imageData.isEmpty() != false); //-V521
                REQUIRE(imageData.getWidth() == 0); //-V521
                REQUIRE(imageData.getHeight() == 0); //-V521

                auto& pixels = imageData.getPixels();
                REQUIRE(pixels.size() == 0); //-V521
            }
            SECTION("assign(valid, nullptr)")
            {
                imageData.assign(newSize, nullptr);

                REQUIRE(imageData.isEmpty() != false); //-V521
                REQUIRE(imageData.getWidth() == 0); //-V521
                REQUIRE(imageData.getHeight() == 0); //-V521

                auto& pixels = imageData.getPixels();
                REQUIRE(pixels.size() == 0); //-V521
            }
            SECTION("assign(empty, valid)")
            {
                imageData.assign(Size(), newPixels);

                REQUIRE(imageData.isEmpty() != false); //-V521
                REQUIRE(imageData.getWidth() == 0); //-V521
                REQUIRE(imageData.getHeight() == 0); //-V521

                auto& pixels = imageData.getPixels();
                REQUIRE(pixels.size() == 0); //-V521
            }
            SECTION("assign(valid, valid)")
            {
                imageData.assign(newSize, newPixels);

                REQUIRE(imageData.isEmpty() == false); //-V521
                REQUIRE(imageData.getWidth() == 1); //-V521
                REQUIRE(imageData.getHeight() == 2); //-V521

                auto& pixels = imageData.getPixels();
                REQUIRE(pixels.size() == 2); //-V521

                for (size_t i = 0; i < pixels.size(); ++i)
                {
                    REQUIRE(newPixels[i].val == pixels[i].val); //-V521
                }
            }
        }
    }
    SECTION("copyToRect")
    {
        Pixel orgPixels1[4][4] =
        {
            0x00000011, 0x00000012, 0x00000013, 0x00000014,
            0x00000021, 0x01020322, 0x01020323, 0x00000024,
            0x00000031, 0x01020332, 0x01020333, 0x00000034,
            0x00000041, 0x00000042, 0x00000043, 0x00000044
        };
        Pixel orgPixels2[4][3] =
        {
            0x00000012, 0x00000013, 0x00000014,
            0x01020322, 0x01020323, 0x00000024,
            0x01020332, 0x01020333, 0x00000034,
            0x00000042, 0x00000043, 0x00000044
        };
        Pixel orgPixels3[4][3] =
        {
            0x00000011, 0x00000012, 0x00000013,
            0x00000021, 0x01020322, 0x01020323,
            0x00000031, 0x01020332, 0x01020333,
            0x00000041, 0x00000042, 0x00000043
        };
        Pixel orgPixels4[4][2] =
        {
            0x00000012, 0x00000013,
            0x01020322, 0x01020323,
            0x01020332, 0x01020333,
            0x00000042, 0x00000043
        };
        Pixel dstPixels[4] =
        {
            0x01020322, 0x01020323,
            0x01020332, 0x01020333
        };

        struct
        {
            Size size;
            const Pixel* pPixels;
            Rect dstRect;

        }
        static data[] =
        {
            {{ 4, 4 }, &orgPixels1[0][0], { 1, 1, 2, 2 }},
            {{ 4, 3 }, &orgPixels1[0][0], { 1, 1, 2, 2 }},
            {{ 4, 3 }, &orgPixels1[1][0], { 1, 0, 2, 2 }},
            {{ 4, 2 }, &orgPixels1[1][0], { 1, 0, 2, 2 }},

            {{ 3, 4 }, &orgPixels2[0][0], { 0, 1, 2, 2 }},
            {{ 3, 3 }, &orgPixels2[0][0], { 0, 1, 2, 2 }},
            {{ 3, 3 }, &orgPixels2[1][0], { 0, 0, 2, 2 }},
            {{ 3, 2 }, &orgPixels2[1][0], { 0, 0, 2, 2 }},

            {{ 3, 4 }, &orgPixels3[0][0], { 1, 1, 2, 2 }},
            {{ 3, 3 }, &orgPixels3[0][0], { 1, 1, 2, 2 }},
            {{ 3, 3 }, &orgPixels3[1][0], { 1, 0, 2, 2 }},
            {{ 3, 2 }, &orgPixels3[1][0], { 1, 0, 2, 2 }},

            {{ 2, 4 }, &orgPixels4[0][0], { 0, 1, 2, 2 }},
            {{ 2, 3 }, &orgPixels4[0][0], { 0, 1, 2, 2 }},
            {{ 2, 3 }, &orgPixels4[1][0], { 0, 0, 2, 2 }},
            {{ 2, 2 }, &orgPixels4[1][0], { 0, 0, 2, 2 }},
        };

        for (const auto& test : data)
        {
            ImageData srcImage(test.size, test.pPixels);

            ImageData dstImage;
            srcImage.copyRect(dstImage, test.dstRect);

            REQUIRE(dstImage.isEmpty() == false); //-V521
            REQUIRE(dstImage.getWidth() == 2); //-V521
            REQUIRE(dstImage.getHeight() == 2); //-V521

            auto& pixels = dstImage.getPixels();
            REQUIRE(pixels.size() == 4); //-V112 //-V521

            for (size_t i = 0; i < pixels.size(); ++i)
            {
                REQUIRE(dstPixels[i].val == pixels[i].val); //-V521
            }

            // Verify original image hasn't changed

            REQUIRE(srcImage.isEmpty() == false); //-V521
            REQUIRE(srcImage.getWidth() == test.size.width); //-V521
            REQUIRE(srcImage.getHeight() == test.size.height); //-V521

            auto& srcPixels = srcImage.getPixels();
            REQUIRE(srcPixels.size() == (test.size.width * test.size.height)); //-V521

            for (size_t i = 0; i < srcPixels.size(); ++i)
            {
                REQUIRE(srcPixels[i].val == test.pPixels[i].val); //-V521
            }
        }
    }
    SECTION("crop")
    {
        Pixel orgPixels1[4][4] =
        {
            0x00000011, 0x00000012, 0x00000013, 0x00000014,
            0x00000021, 0x01020322, 0x01020323, 0x00000024,
            0x00000031, 0x01020332, 0x01020333, 0x00000034,
            0x00000041, 0x00000042, 0x00000043, 0x00000044
        };
        Pixel orgPixels2[4][3] =
        {
            0x00000012, 0x00000013, 0x00000014,
            0x01020322, 0x01020323, 0x00000024,
            0x01020332, 0x01020333, 0x00000034,
            0x00000042, 0x00000043, 0x00000044
        };
        Pixel orgPixels3[4][3] =
        {
            0x00000011, 0x00000012, 0x00000013,
            0x00000021, 0x01020322, 0x01020323,
            0x00000031, 0x01020332, 0x01020333,
            0x00000041, 0x00000042, 0x00000043
        };
        Pixel orgPixels4[4][2] =
        {
            0x00000012, 0x00000013,
            0x01020322, 0x01020323,
            0x01020332, 0x01020333,
            0x00000042, 0x00000043
        };
        Pixel expectedPixels[4] =
        {
            0x01020322, 0x01020323,
            0x01020332, 0x01020333
        };

        struct
        {
            Size size;
            const Pixel* pPixels;
            Rect cropRect;
        }
        static data[] =
        {
            {{ 4, 4 }, &orgPixels1[0][0], { 1, 1, 2, 2 }},
            {{ 4, 3 }, &orgPixels1[0][0], { 1, 1, 2, 2 }},
            {{ 4, 3 }, &orgPixels1[1][0], { 1, 0, 2, 2 }},
            {{ 4, 2 }, &orgPixels1[1][0], { 1, 0, 2, 2 }},

            {{ 3, 4 }, &orgPixels2[0][0], { 0, 1, 2, 2 }},
            {{ 3, 3 }, &orgPixels2[0][0], { 0, 1, 2, 2 }},
            {{ 3, 3 }, &orgPixels2[1][0], { 0, 0, 2, 2 }},
            {{ 3, 2 }, &orgPixels2[1][0], { 0, 0, 2, 2 }},

            {{ 3, 4 }, &orgPixels3[0][0], { 1, 1, 2, 2 }},
            {{ 3, 3 }, &orgPixels3[0][0], { 1, 1, 2, 2 }},
            {{ 3, 3 }, &orgPixels3[1][0], { 1, 0, 2, 2 }},
            {{ 3, 2 }, &orgPixels3[1][0], { 1, 0, 2, 2 }},

            {{ 2, 4 }, &orgPixels4[0][0], { 0, 1, 2, 2 }},
            {{ 2, 3 }, &orgPixels4[0][0], { 0, 1, 2, 2 }},
            {{ 2, 3 }, &orgPixels4[1][0], { 0, 0, 2, 2 }},
            {{ 2, 2 }, &orgPixels4[1][0], { 0, 0, 2, 2 }},
        };

        for (const auto& test : data)
        {
            ImageData image(test.size, test.pPixels);
            image.crop(test.cropRect);

            REQUIRE(image.isEmpty() == false); //-V521
            REQUIRE(image.getWidth() == 2); //-V521
            REQUIRE(image.getHeight() == 2); //-V521

            auto& pixels = image.getPixels();
            REQUIRE(pixels.size() == 4); //-V112 //-V521

            for (size_t i = 0; i < pixels.size(); ++i)
            {
                REQUIRE(expectedPixels[i].val == pixels[i].val); //-V521
            }
        }
    }
    SECTION("getTrimRect (1)")
    {
        Pixel orgPixels1[4][4] =
        {
            0x00000011, 0x00000012, 0x00000013, 0x00000014,
            0x00000021, 0x01020322, 0x01020323, 0x00000024,
            0x00000031, 0x01020332, 0x01020333, 0x00000034,
            0x00000041, 0x00000042, 0x00000043, 0x00000044
        };
        Pixel orgPixels2[4][3] =
        {
            0x00000012, 0x00000013, 0x00000014,
            0x01020322, 0x01020323, 0x00000024,
            0x01020332, 0x01020333, 0x00000034,
            0x00000042, 0x00000043, 0x00000044
        };
        Pixel orgPixels3[4][3] =
        {
            0x00000011, 0x00000012, 0x00000013,
            0x00000021, 0x01020322, 0x01020323,
            0x00000031, 0x01020332, 0x01020333,
            0x00000041, 0x00000042, 0x00000043
        };
        Pixel orgPixels4[4][2] =
        {
            0x00000012, 0x00000013,
            0x01020322, 0x01020323,
            0x01020332, 0x01020333,
            0x00000042, 0x00000043
        };
        Pixel expectedPixels[2][2] =
        {
            0x01020322, 0x01020323,
            0x01020332, 0x01020333
        };

        struct
        {
            Size size;
            const Pixel* pPixels;
            Rect cropRect;
        }
        static data[] =
        {
            {{ 4, 4 }, &orgPixels1[0][0], { 1, 1, 2, 2 }},
            {{ 4, 3 }, &orgPixels1[0][0], { 1, 1, 2, 2 }},
            {{ 4, 3 }, &orgPixels1[1][0], { 1, 0, 2, 2 }},
            {{ 4, 2 }, &orgPixels1[1][0], { 1, 0, 2, 2 }},

            {{ 3, 4 }, &orgPixels2[0][0], { 0, 1, 2, 2 }},
            {{ 3, 3 }, &orgPixels2[0][0], { 0, 1, 2, 2 }},
            {{ 3, 3 }, &orgPixels2[1][0], { 0, 0, 2, 2 }},
            {{ 3, 2 }, &orgPixels2[1][0], { 0, 0, 2, 2 }},

            {{ 3, 4 }, &orgPixels3[0][0], { 1, 1, 2, 2 }},
            {{ 3, 3 }, &orgPixels3[0][0], { 1, 1, 2, 2 }},
            {{ 3, 3 }, &orgPixels3[1][0], { 1, 0, 2, 2 }},
            {{ 3, 2 }, &orgPixels3[1][0], { 1, 0, 2, 2 }},

            {{ 2, 4 }, &orgPixels4[0][0], { 0, 1, 2, 2 }},
            {{ 2, 3 }, &orgPixels4[0][0], { 0, 1, 2, 2 }},
            {{ 2, 3 }, &orgPixels4[1][0], { 0, 0, 2, 2 }},
            {{ 2, 2 }, &orgPixels4[1][0], { 0, 0, 2, 2 }},
        };

        for (const auto& test : data)
        {
            ImageData image(test.size, test.pPixels);
            Rect trimRect = image.getTrimRect();

            REQUIRE(trimRect.isEmpty() == false); //-V521
            REQUIRE(trimRect.width == 2); //-V521
            REQUIRE(trimRect.height == 2); //-V521

            for (uint32_t y = 0; y < trimRect.height; ++y)
            {
                for (uint32_t x = 0; x < trimRect.width; ++x)
                {
                    REQUIRE(expectedPixels[y][x].val == image.getPixel(trimRect.left + x, trimRect.top + y).val); //-V521
                }
            }
        }
    }
    SECTION("getTrimRect (2)")
    {
        Pixel orgPixels[3][5] =
        {
            0x00000011, 0x00000012, 0x00000013, 0x01000014, 0x00000015,
            0x00000021, 0x00000022, 0x00000023, 0x00000024, 0x00000025,
            0x00000031, 0x01000032, 0x00000033, 0x00000034, 0x00000035
        };

        Pixel expectedPixels[3][3] =
        {
            0x00000012, 0x00000013, 0x01000014,
            0x00000022, 0x00000023, 0x00000024,
            0x01000032, 0x00000033, 0x00000034
        };

        ImageData image(Size(5, 3), &orgPixels[0][0]);
        Rect trimRect = image.getTrimRect();

        REQUIRE(trimRect.isEmpty() == false); //-V521
        REQUIRE(trimRect.width == 3); //-V521
        REQUIRE(trimRect.height == 3); //-V521

        for (uint32_t y = 0; y < trimRect.height; ++y)
        {
            for (uint32_t x = 0; x < trimRect.width; ++x)
            {
                REQUIRE(expectedPixels[y][x].val == image.getPixel(trimRect.left + x, trimRect.top + y).val); //-V521
            }
        }
    }
    SECTION("getTrimRect (3)")
    {
        Pixel orgPixels[3][5] =
        {
            0x00000011, 0x01000012, 0x00000013, 0x00000014, 0x00000015,
            0x00000021, 0x00000022, 0x00000023, 0x00000024, 0x00000025,
            0x00000031, 0x00000032, 0x00000033, 0x01000034, 0x00000035
        };

        Pixel expectedPixels[3][3] =
        {
            0x01000012, 0x00000013, 0x00000014,
            0x00000022, 0x00000023, 0x00000024,
            0x00000032, 0x00000033, 0x01000034
        };

        ImageData image(Size(5, 3), &orgPixels[0][0]);
        Rect trimRect = image.getTrimRect();

        REQUIRE(trimRect.isEmpty() == false); //-V521
        REQUIRE(trimRect.width == 3); //-V521
        REQUIRE(trimRect.height == 3); //-V521

        for (uint32_t y = 0; y < trimRect.height; ++y)
        {
            for (uint32_t x = 0; x < trimRect.width; ++x)
            {
                REQUIRE(expectedPixels[y][x].val == image.getPixel(trimRect.left + x, trimRect.top + y).val); //-V521
            }
        }
    }
}