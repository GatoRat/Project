// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Scheduler.h"
#include <sstream>
#include "Util/Json/JsonGet.h"
#include "Util/Json/JsonGetEx2.h"
#include "Util/String/Str.h"

using namespace std;

static const char* weekdayNames[]
{
    "Sunday"   ,
    "Monday"   ,
    "Tuesday"  ,
    "Wednesday",
    "Thursday" ,
    "Friday"   ,
    "Saturday"
};

size_t Schedule::GetWeekdayIndex(const char* pWeekday)
{
    size_t index = 0;

    if (pWeekday)
    {
        for (auto pWeekdayName : weekdayNames)
        {
            if (Str::Compare(pWeekdayName, pWeekday, true) == 0)
            {
                break;
            }
            ++index;
        }
    }
    else
    {
        index = NumWeekdays;
    }

    return index;
}

//static map<const char*, int, str_equal_to_nocase> weekdays
//{
//    { weekdayNames[0], 0 },
//    { weekdayNames[1], 1 },
//    { weekdayNames[2], 2 },
//    { weekdayNames[3], 3 },
//    { weekdayNames[4], 4 },
//    { weekdayNames[5], 5 },
//    { weekdayNames[6], 6 }
//};

bool Schedule::isEmpty() const
{
    return _items.empty() && _begins.isMin() && _ends.isMax();
}

Schedule::Schedule(const rapidjson::Value& jParent)
{
    auto pJsonSchedule = JsonGetObjectEx2(jParent, "schedule");
    if (pJsonSchedule)
    {
        const char* pStr = JsonGetString(*pJsonSchedule, "begins", nullptr);
        if (!Str::IsNullOrEmpty(pStr) && !_begins.set(pStr, false))
        {
            ostringstream os;
            os << "\"begins\" item \"" << pStr << "\" is invalid";
            throw XceptionJson(0, os.str().c_str());
        }

        pStr = JsonGetString(*pJsonSchedule, "ends", nullptr);
        if (!Str::IsNullOrEmpty(pStr) && !_ends.set(pStr, false))
        {
            ostringstream os;
            os << "\"ends\" item \"" << pStr << "\" is invalid";
            throw XceptionJson(0, os.str().c_str());
        }

        const rapidjson::Value* pJsonItems = JsonGetArrayEx2(*pJsonSchedule, "items");
        if (pJsonItems)
        {
            for (const auto& jItem : pJsonItems->GetArray())
            {
                _items.emplace_back(jItem);
            }
        }

        // If there is no _begin or _end AND there is detail, one optimization would
        // be to run the detail list and extract the earliest and latest times.
        /*
        if (_begins.isMin() || _ends.isMax())
        {
            TimeStamp begins{ TimeStamp::Set::ToMin };
            TimeStamp ends  { TimeStamp::Set::ToMax };

            // run list
        }
        */
    }
}

void Schedule::write(JsonWriter& writer) const
{
    JsonWriter::Array scheduleArray("schedule", writer);

    for (auto& item : _items)
    {
        item.write(writer);
    }
}

//bool Schedule::createScore(TimeStamp timeStamp)
//{
//    bool isValid = true;
//
//    if (!_items.empty())
//    {
//        isValid = false;
//
//        for (auto& item : _items)
//        {
//            if (timeStamp <= item._ends)
//            {
//                isValid = true;
//                _score += item.calcScore();
//            }
//        }
//    }
//
//    return isValid;
//}

optional<uint64_t> Schedule::createScore(TimeStamp timeStamp, int dayOfWeek) const
{
    optional<uint64_t> score;

    if (_begins <= timeStamp && timeStamp < _ends)
    {
        if (_items.empty())
        {
            score = MaxNonDefaultScore;
        }
        else
        {
            for (const auto& item : _items)
            {
                if (item.isTimeValid(timeStamp, dayOfWeek))
                {
                    uint64_t tmpScore = item.calcScore();
                    if (!score || tmpScore < *score)
                    {
                        score = tmpScore;
                    }
                }
            }
        }
    }

    return score;
}

bool Schedule::isTimeValid(TimeStamp timeStamp, int dayOfWeek) const
{
    bool isValid = false;

    if (_begins <= timeStamp && timeStamp < _ends)
    {
        if (_items.empty())
        {
            isValid = true;
        }
        else
        {
            for (const auto& item : _items)
            {
                if (item.isTimeValid(timeStamp, dayOfWeek))
                {
                    isValid = true;
                    break;
                }
            }
        }
    }

    return isValid;
}

vector<TimeStamp> Schedule::getPreemptTimes(TimeStamp curTime, int dayOfWeek) const
{
    vector<TimeStamp> timeStamps;

    if (!_items.empty() && _begins <= curTime && curTime < _ends)
    {
        for (const auto& item : _items)
        {
            TimeStamp preemptTime;
            if (item.getPreemptTime(curTime, dayOfWeek, preemptTime))
            {
                timeStamps.push_back(preemptTime);
            }
        }
    }

    return timeStamps;
}

Schedule::Item::Item(const rapidjson::Value& jItem)
{
    const char* pStr = JsonGetString(jItem, "begins", nullptr);
    if (!Str::IsNullOrEmpty(pStr) && !_begins.set2(pStr, false))
    {
        ostringstream os;
        os << "\"begins\" item \"" << pStr << "\" is invalid";
        throw XceptionJson(0, os.str().c_str());
    }

    pStr = JsonGetString(jItem, "ends", nullptr);
    if (!Str::IsNullOrEmpty(pStr) && !_ends.set2(pStr, false))
    {
        ostringstream os;
        os << "\"ends\" item \"" << pStr << "\" is invalid";
        throw XceptionJson(0, os.str().c_str());
    }

    const rapidjson::Value* pJsonDays = JsonGetArrayEx2(jItem, "days");
    if (pJsonDays && !pJsonDays->GetArray().Empty())
    {
        _daysOfWeek.reset();

        for (const auto& jDay : pJsonDays->GetArray())
        {
            if (!jDay.IsString())
            {
                throw XceptionJson(0, "\"days\" item is not a string");
            }

            pStr = jDay.GetString();
            const auto index = GetWeekdayIndex(pStr);
            if (index >= NumWeekdays)
            {
                ostringstream os;
                os << "\"days\" item \"" << pStr << "\" is not a weekday";
                throw XceptionJson(0, os.str().c_str());
            }

            _daysOfWeek.set(index);
        }
    }

    _preempt = JsonGetBoolEx2(jItem, "preempt");
}

void Schedule::Item::write(JsonWriter& writer) const
{
    if (!isDefault())
    {
        JsonWriter::Object jItem(writer);

        if (!_begins.isMin())
        {
            writer.keyVal("begins", _begins.toStr());
        }

        if (!_ends.isMax())
        {
            writer.keyVal("ends", _ends.toStr());
        }

        if (_daysOfWeek.count() != NumWeekdays)
        {
            JsonWriter::Array dayArray("days", writer);

            for (size_t day = 0; day < NumWeekdays; ++day)
            {
                if (_daysOfWeek.test(day))
                {
                    writer.String(weekdayNames[day]);
                }
            }
        }
    }
}

uint64_t Schedule::Item::calcScore() const
{
    uint64_t score = _begins.isMin() || _ends.isMax() || _ends.val < _begins.val ? MaxNonDefaultScore : _ends.val - _begins.val;

    if (_daysOfWeek.count() < NumWeekdays)
    {
        TimeStamp timeStamp{ 0, 0, static_cast<uint8_t>(_daysOfWeek.count()), 0, 0, 0 };

        if (score > timeStamp.val)
        {
            score = timeStamp.val;
        }
    }

    return score > MaxNonDefaultScore ? MaxNonDefaultScore : score;
}

bool Schedule::Item::isDayOfWeekValid(int dayOfWeek) const
{
    return static_cast<size_t>(dayOfWeek) < NumWeekdays && _daysOfWeek[dayOfWeek]; //-V108
}

bool Schedule::Item::isTimeValid(TimeStamp timeStamp, int dayOfWeek) const
{
    bool isValid = false;

    if (isDayOfWeekValid(dayOfWeek))
    {
        if (_begins.isTimeOnly() || _ends.isTimeOnly())
        {
            TimeStamp begins{ timeStamp, _begins };
            TimeStamp ends  { timeStamp, _ends   };

            isValid = begins <= timeStamp && timeStamp < ends;
        }
        else
        {
            isValid = _begins <= timeStamp && timeStamp < _ends;
        }
    }

    return isValid;
}

bool Schedule::Item::getPreemptTime(TimeStamp curTime, int dayOfWeek, TimeStamp& preemptTime) const
{
    bool isValid = false;

    if (_preempt && static_cast<size_t>(dayOfWeek) < NumWeekdays && _daysOfWeek[dayOfWeek]) //-V108
    {
        if (_begins.isTimeOnly())
        {
            TimeStamp begins{ curTime, _begins };

            if (curTime <= begins)
            {
                isValid = true;
                preemptTime = begins;
            }
        }
        else
        {
            if (curTime <= _begins)
            {
                TimeStamp endOfDay(curTime);

                endOfDay.dt.hour = TimeStamp::MaxHour;
                endOfDay.dt.min  = TimeStamp::MaxMinute;
                endOfDay.dt.sec  = TimeStamp::MaxSeconds;
                endOfDay.dt.msec = TimeStamp::MaxMilliseconds;

                if (_begins <= endOfDay)
                {
                    isValid = true;
                    preemptTime = _begins;
                }
            }
        }
    }

    return isValid;
}

bool Schedule::Item::isDefault() const
{
    return _begins.isMin() && _ends.isMax() && _daysOfWeek.count() == NumWeekdays;

}

//unique_ptr<Schedule> Schedule::ReadSchedule(const rapidjson::Value& jParent)
//{
//    unique_ptr<Schedule> schedule;
//
//    auto memberIt = jParent.FindMember("schedule");
//    if (memberIt == jParent.MemberEnd() || memberIt->value.IsNull())
//    {
//    }
//    else if (!memberIt->value.IsObject())
//    {
//        throw XceptionJson(JsonError::ValueIsNotObject, "schedule");
//    }
//    else
//    {
//        schedule = make_unique<Schedule>(memberIt->value);
//    }
//
//    return schedule;
//}