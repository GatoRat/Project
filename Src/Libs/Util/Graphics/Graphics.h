// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cstdint>
#include <ostream>
#include <string>
#include <RapidJSON/rapidjson.h>
#include <RapidJSON/document.h>
#include "../Json/JsonWriter.h"

namespace Graphics
{
    struct RectF;
    struct Size;
    struct SizeF;
    struct Position;
    struct PositionF;

    struct Rect
    {
        uint32_t left   = 0;
        uint32_t top    = 0;
        uint32_t width  = 0;
        uint32_t height = 0;

        Rect() = default;
        Rect(uint32_t left, uint32_t top, uint32_t width, uint32_t height);
        Rect(Position pos, Size size); // deliberately pass by value
        explicit Rect(const RectF& rect);
        Rect(const rapidjson::Value& jsonVal, const char* pName);

        void set(uint32_t left, uint32_t top, uint32_t width, uint32_t height);
        void set(Position pos, Size size);
        void set(const RectF& rect);

        void clear();

        enum Corner
        {
            Top    = 0b00,
            Bottom = 0b01,

            Left   = 0b00,
            Right  = 0b10,

            TopLeft     = Top | Left,
            TopRight    = Top | Right,
            BottomLeft  = Bottom | Left,
            BottomRight = Bottom | Right
        };

        Position get(Corner corner) const;
        operator Position() const;

        Size getSize() const;
        operator Size() const;

        uint32_t right() const;
        uint32_t bottom() const;

        Position center() const;

        bool isEmpty() const;

        bool contains(Position pos) const;
        bool contains(uint32_t x, uint32_t y) const;

        const Rect& combine(const Rect& rect) const;
        const Rect& operator+=(const Rect& rect);

        bool intersects(const Rect& rect) const;
        bool intersects(const Rect& rect, Rect& intersection) const;

        void read(const rapidjson::Value& jsonVal, const char* pName);
        void write(JsonWriter& writer, const char* pName, bool abbreviated = false) const;
    };

    bool operator==(const Rect& rect1, const Rect& rect2);
    std::string Format(const Rect& rect);
    std::ostream& operator<<(std::ostream& stream, const Rect& rect);

    ///////////////////////////////////////////////////////////////////////

    struct RectF
    {
        float left   = 0.0f;
        float top    = 0.0f;
        float width  = 0.0f;
        float height = 0.0f;

        RectF() = default;
        RectF(float left, float top, float width, float height);
        RectF(PositionF pos, SizeF size); // deliberately pass by value
        explicit RectF(const Rect& rect);
        RectF(const rapidjson::Value& jsonVal, const char* pName);

        void set(float left, float top, float width, float height);
        void set(PositionF pos, SizeF size);
        void set(const Rect& rect);

        PositionF get(Rect::Corner corner) const;
        operator PositionF() const;

        SizeF getSize() const;
        operator SizeF() const;

        float right() const;
        float bottom() const;

        PositionF center() const;

        bool isEmpty() const;

        void read(const rapidjson::Value& jsonVal, const char* pName);
        void write(JsonWriter& writer, const char* pName, bool abbreviated = false) const;
    };

    bool operator==(const RectF& rect1, const RectF& rect2);
    std::string Format(const RectF& rect, std::streamsize precision = 4);
    std::ostream& operator<<(std::ostream& stream, const RectF& rect);

    ///////////////////////////////////////////////////////////////////////

    struct Position
    {
        uint32_t x = 0;
        uint32_t y = 0;

        Position() = default;
        Position(uint32_t x, uint32_t y);
        explicit Position(PositionF position);
        Position(const rapidjson::Value& jsonVal, const char* pName);

        void set(uint32_t x, uint32_t y);
        void set(Position position);
        void set(PositionF position);

        void clear();

        bool isZero() const;

        void read(const rapidjson::Value& jsonVal, const char* pName);
        void write(JsonWriter& writer, const char* pName) const;
    };

    bool operator==(const Position& position1, const Position& position2);
    std::string Format(Position position);
    std::ostream& operator<<(std::ostream& stream, Position position);

    ///////////////////////////////////////////////////////////////////////

    struct PositionF
    {
        float x = 0.0f;
        float y = 0.0f;

        PositionF() = default;
        PositionF(float x, float y);
        explicit PositionF(Position position);
        PositionF(const rapidjson::Value& jsonVal, const char* pName);

        void set(float x, float y);
        void set(Position position);
        void set(PositionF position);

        void clear();

        bool isZero() const;

        void read(const rapidjson::Value& jsonVal, const char* pName);
        void write(JsonWriter& writer, const char* pName) const;
    };

    bool operator==(const PositionF& position1, const PositionF& position2);
    std::string Format(PositionF position, std::streamsize precision = 4);
    std::ostream& operator<<(std::ostream& stream, PositionF position);

    ///////////////////////////////////////////////////////////////////////

    struct Size
    {
        uint32_t width  = 0;
        uint32_t height = 0;

        Size() = default;
        Size(uint32_t width, uint32_t height);
        explicit Size(SizeF size);
        Size(const rapidjson::Value& jsonVal, const char* pName);

        void set(uint32_t width, uint32_t height);
        void set(Size size);
        void set(SizeF size);

        void clear();

        Position center() const;

        bool isEmpty() const;

        void read(const rapidjson::Value& jsonVal, const char* pName);
        void write(JsonWriter& writer, const char* pName, bool abbreviated = false) const;
    };

    bool operator==(const Size& size1, const Size& size2);
    std::string Format(Size size);
    std::ostream& operator<<(std::ostream& stream, Size size);

    ///////////////////////////////////////////////////////////////////////

    struct SizeF
    {
        float width  = 0.0f;
        float height = 0.0f;

        SizeF() = default;
        SizeF(float width, float height);
        explicit SizeF(Size size);
        SizeF(const rapidjson::Value& jsonVal, const char* pName);

        void set(float width, float height);
        void set(SizeF size);
        void set(Size size);

        void clear();

        PositionF center() const;

        bool isEmpty() const;

        void read(const rapidjson::Value& jsonVal, const char* pName);
        void write(JsonWriter& writer, const char* pName, bool abbreviated = false) const;
    };

    bool operator==(const SizeF& size1, const SizeF& size2);
    std::string Format(SizeF size, std::streamsize precision = 4);
    std::ostream& operator<<(std::ostream& stream, SizeF size);

    ///////////////////////////////////////////////////////////////////////

    struct BGRA
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;
    };

    union Pixel
    {
        uint32_t val;
        BGRA     bgra;

        Pixel() = default;

        constexpr Pixel(uint32_t val_)
            : val{ val_ }
        {
        }

        constexpr Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
            : bgra{ blue, green, red, alpha }
        {
            if (!alpha)
            {
                val = 0;
            }
        }
    };

    constexpr Pixel BlackPixel{   0,   0,   0, 255 };
    constexpr Pixel WhitePixel{ 255, 255, 255, 255 };

} // namespace Graphics
