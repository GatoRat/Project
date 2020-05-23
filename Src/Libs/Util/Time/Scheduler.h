// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <bitset>
#include <optional>
#include <vector>
#include <RapidJSON/rapidjson.h>
#include <RapidJSON/document.h>
#include "Util/Json/JsonWriter.h"
#include "Util/Time/TimeStamp.h"

class Schedule
{
public:
    Schedule() = default;
    explicit Schedule(const rapidjson::Value& jParent);
    ~Schedule() = default;

    bool isEmpty() const;

    void write(JsonWriter& writer) const;

    std::optional<uint64_t> createScore(TimeStamp timeStamp, int dayOfWeek) const;

    bool isTimeValid(TimeStamp timeStamp, int dayOfWeek) const;

    std::vector<TimeStamp> getPreemptTimes(TimeStamp curTime, int dayOfWeek) const;

    static constexpr size_t NumWeekdays{ 7 };
    static size_t GetWeekdayIndex(const char* pWeekday);

    static constexpr uint64_t MaxDefaultScore   { UINT64_MAX };
    static constexpr uint64_t MaxNonDefaultScore{ MaxDefaultScore - 1 };

    //static std::unique_ptr<Schedule> ReadSchedule(const rapidjson::Value& jParent);

private:
    TimeStamp _begins{ TimeStamp::Set::ToMin };
    TimeStamp _ends  { TimeStamp::Set::ToMax };

    struct Item
    {
        TimeStamp _begins{ TimeStamp::Set::ToMin };
        TimeStamp _ends  { TimeStamp::Set::ToMax };
        std::bitset<NumWeekdays> _daysOfWeek{ 0x7f };
        bool      _preempt = false;

        explicit Item(const rapidjson::Value& jItem);

        void write(JsonWriter& writer) const;

        uint64_t calcScore() const;

        bool isTimeValid(TimeStamp timeStamp, int dayOfWeek) const;
        bool isDayOfWeekValid(int dayOfWeek) const;
        bool getPreemptTime(TimeStamp curTime, int dayOfWeek, TimeStamp& preemptTime) const;
        bool isDefault() const;
    };

    std::vector<Item> _items;
};
