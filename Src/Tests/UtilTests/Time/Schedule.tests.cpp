// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <sstream>
#include <RapidJSON/error/en.h>
#include "Util/Time/Scheduler.h"
#include "TestHelper/catch.hpp"

using namespace std;

static char* pCtor1 = R"(
{
  "schedule":
  {
    "begins":"2018-04-20 10:11",
    "ends":"2018-04-21 12:35",
    "items":
    [
      {
        "preempt": true,
        "begins": "12:30:00",
        "ends": "12:34:56",
        "days": ["Monday","Wednesday","Friday"]
      }
    ]
  }
})";

TEST_CASE("Misc/Schedule")
{
    SECTION("ctor(json)")
    {
        rapidjson::Document doc;
        if (doc.Parse(pCtor1).HasParseError())
        {
            stringstream ss;
            ss << "Error: " <<
                rapidjson::GetParseError_En(doc.GetParseError()) <<
                ": offset=" << doc.GetErrorOffset();
            FAIL(ss.str().c_str());
        }

        Schedule schedule(doc);

        REQUIRE_FALSE(schedule.isEmpty()); //-V521

        {
            TimeStamp early(2018, 4, 20, 10, 10, 59); //-V112

            REQUIRE_FALSE(schedule.isTimeValid(early, 0)); //-V521
            REQUIRE_FALSE(schedule.isTimeValid(early, 1)); //-V521
        }
        {
            TimeStamp goodTime(2018, 4, 20, 12, 31, 0); //-V112

            REQUIRE_FALSE(schedule.isTimeValid(goodTime, 0)); //-V521
            REQUIRE(schedule.isTimeValid(goodTime, 1)); //-V521
        }
        {
            TimeStamp goodTime(2018, 4, 21, 12, 34, 00); //-V112

            REQUIRE_FALSE(schedule.isTimeValid(goodTime, -1)); //-V521
            REQUIRE_FALSE(schedule.isTimeValid(goodTime, 0)); //-V521
            REQUIRE_FALSE(schedule.isTimeValid(goodTime, 2)); //-V521
            REQUIRE_FALSE(schedule.isTimeValid(goodTime, 4)); //-V112 //-V521
            REQUIRE_FALSE(schedule.isTimeValid(goodTime, 6)); //-V521
            REQUIRE_FALSE(schedule.isTimeValid(goodTime, 7)); //-V521

            REQUIRE(schedule.isTimeValid(goodTime, 1)); //-V521
            REQUIRE(schedule.isTimeValid(goodTime, 3)); //-V521
            REQUIRE(schedule.isTimeValid(goodTime, 5)); //-V521
        }
        {
            TimeStamp goodTime(2018, 4, 21, 12, 34, 00); //-V112
            vector<TimeStamp> preemptTimes = schedule.getPreemptTimes(goodTime, 1);
            REQUIRE(preemptTimes.empty()); //-V521
        }
        {
            TimeStamp goodTime(2018, 4, 21, 11, 34, 00); //-V112
            vector<TimeStamp> preemptTimes = schedule.getPreemptTimes(goodTime, 1);
            REQUIRE(preemptTimes.size() == 1); //-V521
        }

        //Schedule
    }
    SECTION("createScore")
    {
        SECTION("default")
        {
            Schedule schedule;

            TimeStamp goodTime(2018, 4, 21, 12, 00, 00); //-V112
            auto score = schedule.createScore(goodTime, 0);
            REQUIRE(score.has_value()); //-V521
            REQUIRE(score.value() == UINT64_MAX - 1); //-V521
        }
    }
}
