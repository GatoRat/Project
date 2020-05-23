// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Graphics.h"
#include <sstream>
#include "../Json/JsonGetEx.h"

using namespace std;

namespace Graphics
{
    //rapidjson::Value::ConstMemberIterator JsonFindNumberMemberEx2(const rapidjson::Value& value, const char* pName1, const char* pName2)
    //{
    //    const char* pNameUsed = pName1;

    //    auto memberIt = value.FindMember(pNameUsed);
    //    if (memberIt == value.MemberEnd())
    //    {
    //        pNameUsed = pName2;
    //        memberIt = value.FindMember(pNameUsed);
    //        if (memberIt == value.MemberEnd())
    //        {
    //            string name(pName1);
    //            name += " or ";
    //            name += pName2;

    //            throw XceptionJson(JsonError::MemberNotFound, name.c_str());
    //        }
    //    }

    //    if (!memberIt->value.IsNumber())
    //    {
    //        throw XceptionJson(JsonError::ValueIsNotNumber, pNameUsed);
    //    }

    //    return memberIt;
    //}

    //uint32_t JsonGetUintEx2(const rapidjson::Value& value, const char* pName1, const char* pName2)
    //{
    //    return JsonFindNumberMemberEx2(value, pName1, pName2)->value.GetUint();
    //}

    //float JsonGetFloatEx2(const rapidjson::Value& value, const char* pName1, const char* pName2)
    //{
    //    return JsonFindNumberMemberEx2(value, pName1, pName2)->value.GetFloat();
    //}

    Rect::Rect(uint32_t left_, uint32_t top_, uint32_t width_, uint32_t height_)
        : left(left_)
        , top(top_)
        , width(width_)
        , height(height_)
    {
    }

    Rect::Rect(Position pos, Size size)
        : left(pos.x)
        , top(pos.y)
        , width(size.width)
        , height(size.height)
    {
    }

    Rect::Rect(const RectF& rect)
        : left  (static_cast<uint32_t>(rect.left))
        , top   (static_cast<uint32_t>(rect.top))
        , width (static_cast<uint32_t>(rect.width))
        , height(static_cast<uint32_t>(rect.height))
    {
    }

    Rect::Rect(const rapidjson::Value& jsonValue, const char* pName)
    {
        read(jsonValue, pName);
    }

    void Rect::set(uint32_t left_, uint32_t top_, uint32_t width_, uint32_t height_)
    {
        left   = left_;
        top    = top_;
        width  = width_;
        height = height_;
    }

    void Rect::set(Position pos, Size size)
    {
        left   = pos.x;
        top    = pos.y;
        width  = size.width;
        height = size.height;
    }

    void Rect::set(const RectF& rect)
    {
        left   = static_cast<uint32_t>(rect.left);
        top    = static_cast<uint32_t>(rect.top);
        width  = static_cast<uint32_t>(rect.width);
        height = static_cast<uint32_t>(rect.height);
    }

    void Rect::clear()
    {
        set(0, 0, 0, 0);
    }

    Position Rect::get(Corner corner) const
    {
        return Position((corner & Right) ? left + width : left,
                        (corner & Bottom) ? top + height : top);
    }

    Rect::operator Position() const
    {
        return Position(left, top);
    }

    Size Rect::getSize() const
    {
        return Size(width, height);
    }

    Rect::operator Size() const
    {
        return Size(width, height);
    }

    uint32_t Rect::right() const
    {
        return left + width;
    }

    uint32_t Rect::bottom() const
    {
        return top + height;
    }

    Position Rect::center() const
    {
        return Position(left + (width / 2), top + (height / 2));
    }

    bool Rect::isEmpty() const
    {
        return width == 0 || height == 0;
    }

    bool Rect::contains(Position pos) const
    {
        return contains(pos.x, pos.y);
    }

    bool Rect::contains(uint32_t x, uint32_t y) const
    {
        return x >= left && x < right() && y >= top && y < bottom();
    }

    const Rect& Rect::combine(const Rect& /*rect*/) const
    {
        return *this;
    }

    const Rect& Rect::operator+=(const Rect& rect)
    {
        return combine(rect);
    }

    bool Rect::intersects(const Rect& rect) const
    {
        return rect.left < right() && rect.top < bottom() &&
               rect.right() >= left && rect.bottom() >= top;
    }

    bool Rect::intersects(const Rect& rect, Rect& intersection) const
    {
        if (intersects(rect))
        {
            /// ????
        }
        else
        {
            intersection.clear();
        }

        return intersection.width != 0;
    }

    void Rect::read(const rapidjson::Value& jsonVal, const char* pName)
    {
        const rapidjson::Value& value = pName ? JsonGetObjectEx(jsonVal, pName) : jsonVal;

        left   = JsonGetUintEx_two(value, "x", "left");
        top    = JsonGetUintEx_two(value, "y", "top");
        width  = JsonGetUintEx_two(value, "w", "width");
        height = JsonGetUintEx_two(value, "h", "height");
    }

    void Rect::write(JsonWriter& writer, const char* pName, bool abbreviated) const
    {
        JsonWriter::Object jObj(pName, writer);

        if (abbreviated)
        {
            writer.keyVal("x", left);
            writer.keyVal("y", top);
            writer.keyVal("w", width);
            writer.keyVal("h", height);
        }
        else
        {
            writer.keyVal("left"  , left);
            writer.keyVal("top"   , top);
            writer.keyVal("width" , width);
            writer.keyVal("height", height);
        }
    }

    bool operator==(const Rect& rect1, const Rect& rect2)
    {
        return rect1.left  == rect2.left  && rect1.top    == rect2.top &&
               rect1.width == rect2.width && rect1.height == rect2.height;
    }

    string Format(const Rect& rect)
    {
        string str;
        str.reserve(64);

        str += to_string(rect.left);
        str += ',';
        str += to_string(rect.top);
        str += ',';
        str += to_string(rect.width);
        str += ',';
        str += to_string(rect.height);

        return str;
    }

    ostream& operator<<(ostream& stream, const Rect& rect)
    {
        stream << Format(rect);
        return stream;
    }

    ///////////////////////////////////////////////////////////////////////

    RectF::RectF(float left_, float top_, float width_, float height_)
        : left(left_)
        , top(top_)
        , width(width_)
        , height(height_)
    {
    }

    RectF::RectF(PositionF pos, SizeF size)
        : left(pos.x)
        , top(pos.y)
        , width(size.width)
        , height(size.height)
    {
    }

    RectF::RectF(const Rect& rect)
        : left  (static_cast<float>(rect.left))
        , top   (static_cast<float>(rect.top))
        , width (static_cast<float>(rect.width))
        , height(static_cast<float>(rect.height))
    {
    }

    RectF::RectF(const rapidjson::Value& jsonValue, const char* pName)
    {
        read(jsonValue, pName);
    }

    void RectF::set(float left_, float top_, float width_, float height_)
    {
        left   = left_;
        top    = top_;
        width  = width_;
        height = height_;
    }

    void RectF::set(PositionF pos, SizeF size)
    {
        left   = pos.x;
        top    = pos.y;
        width  = size.width;
        height = size.height;
    }

    void RectF::set(const Rect& rect)
    {
        left   = static_cast<float>(rect.left);
        top    = static_cast<float>(rect.top);
        width  = static_cast<float>(rect.width);
        height = static_cast<float>(rect.height);
    }

    PositionF RectF::get(Rect::Corner corner) const
    {
        return PositionF((corner & Rect::Right) ? left + width : left,
                         (corner & Rect::Bottom) ? top + height : top);
    }

    RectF::operator PositionF() const
    {
        return PositionF(left, top);
    }

    SizeF RectF::getSize() const
    {
        return SizeF(width, height);
    }

    RectF::operator SizeF() const
    {
        return SizeF(width, height);
    }

    float RectF::right() const
    {
        return left + width;
    }

    float RectF::bottom() const
    {
        return top + height;
    }

    PositionF RectF::center() const
    {
        return PositionF(left + (width / 2.0f), top + (height / 2.0f));
    }

    bool RectF::isEmpty() const
    {
        return width < Almost::Tolerance || height < Almost::Tolerance;
    }

    void RectF::read(const rapidjson::Value& jsonVal, const char* pName)
    {
        const rapidjson::Value& value = pName ? JsonGetObjectEx(jsonVal, pName) : jsonVal;

        left   = JsonGetFloatEx_two(value, "x", "left");
        top    = JsonGetFloatEx_two(value, "y", "top");
        width  = JsonGetFloatEx_two(value, "w", "width");
        height = JsonGetFloatEx_two(value, "h", "height");
    }

    void RectF::write(JsonWriter& writer, const char* pName, bool abbreviated) const
    {
        JsonWriter::Object jObj(pName, writer);

        if (abbreviated)
        {
            writer.keyVal("x", left);
            writer.keyVal("y", top);
            writer.keyVal("w", width);
            writer.keyVal("h", height);
        }
        else
        {
            writer.keyVal("left"  , left);
            writer.keyVal("top"   , top);
            writer.keyVal("width" , width);
            writer.keyVal("height", height);
        }
    }

    bool operator==(const RectF& rect1, const RectF& rect2)
    {
        return Almost::Equal(rect1.left  , rect2.left)  &&
               Almost::Equal(rect1.top   , rect2.top)   &&
               Almost::Equal(rect1.width , rect2.width) &&
               Almost::Equal(rect1.height, rect2.height);
    }

    string Format(const RectF& rect, streamsize precision)
    {
        stringstream ss;

        ss.precision(precision);
        ss  << fixed << rect.left  << ',' << rect.top << ','
                     << rect.width << ',' << rect.height;

        return ss.str();
    }

    ostream& operator<<(ostream& stream, const RectF& rect)
    {
        stream << Format(rect);
        return stream;
    }

    ///////////////////////////////////////////////////////////////////////

    Position::Position(uint32_t x_, uint32_t y_)
        : x(x_)
        , y(y_)
    {
    }

    Position::Position(const rapidjson::Value& jsonVal, const char* pName)
    {
        read(jsonVal, pName);
    }

    Position::Position(PositionF position)
        : x(static_cast<uint32_t>(position.x))
        , y(static_cast<uint32_t>(position.y))
    {
    }

    void Position::set(uint32_t x_, uint32_t y_)
    {
        x = x_;
        y = y_;
    }

    void Position::set(Position position)
    {
        x = position.x;
        y = position.y;
    }

    void Position::set(PositionF position)
    {
        x = static_cast<uint32_t>(position.x);
        y = static_cast<uint32_t>(position.y);
    }

    void Position::clear()
    {
        x = 0;
        y = 0;
    }

    bool Position::isZero() const
    {
        return x == 0 && y == 0;
    }

    void Position::read(const rapidjson::Value& jsonVal, const char* pName)
    {
        const rapidjson::Value& value = pName ? JsonGetObjectEx(jsonVal, pName) : jsonVal;

        x = JsonGetUintEx(value, "x");
        y = JsonGetUintEx(value, "y");
    }

    void Position::write(JsonWriter& writer, const char* pName) const
    {
        JsonWriter::Object jObj(pName, writer);

        writer.keyVal("x", x);
        writer.keyVal("y", y);
    }

    bool operator==(const Position& position1, const Position& position2)
    {
        return position1.x == position2.x && position1.y == position2.y;
    }

    string Format(Position val)
    {
        string str;
        str.reserve(32); //-V112

        str += to_string(val.x);
        str += ',';
        str += to_string(val.y);

        return str;
    }

    ostream& operator<<(ostream& stream, Position val)
    {
        stream << Format(val);
        return stream;
    }

    ///////////////////////////////////////////////////////////////////////

    PositionF::PositionF(float x_, float y_)
        : x(x_)
        , y(y_)
    {
    }

    PositionF::PositionF(Position position)
        : x(static_cast<float>(position.x))
        , y(static_cast<float>(position.y))
    {
    }

    PositionF::PositionF(const rapidjson::Value& jsonVal, const char* pName)
    {
        read(jsonVal, pName);
    }

    void PositionF::set(float x_, float y_)
    {
        x = x_;
        y = y_;
    }

    void PositionF::set(PositionF position)
    {
        x = position.x;
        y = position.y;
    }

    void PositionF::set(Position position)
    {
        x = static_cast<float>(position.x);
        y = static_cast<float>(position.y);
    }

    void PositionF::clear()
    {
        x = 0.0f;
        y = 0.0f;
    }

    bool PositionF::isZero() const
    {
        return Almost::Zero(x) && Almost::Zero(y);
    }

    void PositionF::read(const rapidjson::Value& jsonVal, const char* pName)
    {
        const rapidjson::Value& value = pName ? JsonGetObjectEx(jsonVal, pName) : jsonVal;

        x = JsonGetFloatEx(value, "x");
        y = JsonGetFloatEx(value, "y");
    }

    void PositionF::write(JsonWriter& writer, const char* pName) const
    {
        JsonWriter::Object jObj(pName, writer);

        writer.keyVal("x", x);
        writer.keyVal("y", y);
    }

    bool operator==(const PositionF& position1, const PositionF& position2)
    {
        return Almost::Equal(position1.x, position2.x)  &&
               Almost::Equal(position1.y, position2.y);
    }

    string Format(PositionF position, streamsize precision)
    {
        stringstream ss;

        ss.precision(precision);
        ss << fixed << position.x << ',' << position.y;

        return ss.str();
    }

    ostream& operator<<(ostream& stream, PositionF position)
    {
        stream << Format(position);
        return stream;
    }

    ///////////////////////////////////////////////////////////////////////

    Size::Size(uint32_t width_, uint32_t height_)
        : width(width_)
        , height(height_)
    {
    }

    Size::Size(SizeF size)
        : width (static_cast<uint32_t>(size.width))
        , height(static_cast<uint32_t>(size.height))
    {
    }

    Size::Size(const rapidjson::Value& jsonVal, const char* pName)
    {
        read(jsonVal, pName);
    }

    void Size::set(uint32_t width_, uint32_t height_)
    {
        width  = width_;
        height = height_;
    }

    void Size::set(Size size)
    {
        width  = size.width;
        height = size.height;
    }

    void Size::set(SizeF size)
    {
        width  = static_cast<uint32_t>(size.width);
        height = static_cast<uint32_t>(size.height);
    }

    Position Size::center() const
    {
        return Position(width / 2, height / 2);
    }

    void Size::clear()
    {
        width  = 0;
        height = 0;
    }

    bool Size::isEmpty() const
    {
        return width == 0 || height == 0;
    }

    void Size::read(const rapidjson::Value& jsonVal, const char* pName)
    {
        const rapidjson::Value& value = pName ? JsonGetObjectEx(jsonVal, pName) : jsonVal;

        width  = JsonGetUintEx_two(value, "w", "width");
        height = JsonGetUintEx_two(value, "h", "height");
    }

    void Size::write(JsonWriter& writer, const char* pName, bool abbreviated) const
    {
        JsonWriter::Object jObj(pName, writer);

        if (abbreviated)
        {
            writer.keyVal("w", width);
            writer.keyVal("h", height);
        }
        else
        {
            writer.keyVal("width", width);
            writer.keyVal("height", height);
        }
    }

    bool operator==(const Size& size1, const Size& size2)
    {
        return size1.width == size2.width && size1.height == size2.height;
    }

    string Format(Size size)
    {
        string str;
        str.reserve(64);

        str += to_string(size.width);
        str += ',';
        str += to_string(size.height);

        return str;
    }

    ostream& operator<<(ostream& stream, Size size)
    {
        stream << Format(size);
        return stream;
    }

    ///////////////////////////////////////////////////////////////////////

    SizeF::SizeF(float width_, float height_)
        : width(width_)
        , height(height_)
    {
    }

    SizeF::SizeF(Size size)
        : width (static_cast<float>(size.width))
        , height(static_cast<float>(size.height))
    {
    }

    SizeF::SizeF(const rapidjson::Value& jsonVal, const char* pName)
    {
        read(jsonVal, pName);
    }

    void SizeF::set(float width_, float height_)
    {
        width  = width_;
        height = height_;
    }

    void SizeF::set(SizeF size)
    {
        width  = size.width;
        height = size.height;
    }

    void SizeF::set(Size size)
    {
        width  = static_cast<float>(size.width);
        height = static_cast<float>(size.height);
    }

    PositionF SizeF::center() const
    {
        return PositionF(width / 2.0f, height / 2.0f);
    }

    bool SizeF::isEmpty() const
    {
        return Almost::Zero(width) || Almost::Zero(height);
    }

    void SizeF::clear()
    {
        width = 0.0f;
        height = 0.0f;
    }

    void SizeF::read(const rapidjson::Value& jsonVal, const char* pName)
    {
        const rapidjson::Value& value = pName ? JsonGetObjectEx(jsonVal, pName) : jsonVal;

        width  = JsonGetFloatEx_two(value, "w", "width");
        height = JsonGetFloatEx_two(value, "h", "height");
    }

    void SizeF::write(JsonWriter& writer, const char* pName, bool abbreviated) const
    {
        JsonWriter::Object jObj(pName, writer);

        if (abbreviated)
        {
            writer.keyVal("w", width);
            writer.keyVal("h", height);
        }
        else
        {
            writer.keyVal("width" , width);
            writer.keyVal("height", height);
        }
    }

    bool operator==(const SizeF& size1, const SizeF& size2)
    {
        return Almost::Equal(size1.width , size2.width)  &&
               Almost::Equal(size1.height, size2.height);
    }

    string Format(SizeF size, streamsize precision)
    {
        stringstream ss;

        ss.precision(precision);
        ss << fixed << size.width << ',' << size.height;

        return ss.str();
    }

    ostream& operator<<(ostream& stream, SizeF size)
    {
        stream << Format(size);
        return stream;
    }

} // namespace Graphics
