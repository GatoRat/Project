// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

// clang-format off

// Except for HUGE numbers, these are defines rather than constexpr so they can be used with any type
// An enum wasn't used because that's just lame in this case.

#define NANOSECONDS_PER_MICROSECOND              1'000
#define NANOSECONDS_PER_MILLISECOND          1'000'000
#define NANOSECONDS_PER_CENTISECOND         10'000'000
#define NANOSECONDS_PER_DECISECOND         100'000'000
#define NANOSECONDS_PER_SECOND           1'000'000'000

#define MICROSECONDS_PER_MILLISECOND             1'000
#define MICROSECONDS_PER_CENTISECOND            10'000
#define MICROSECONDS_PER_DECISECOND            100'000
#define MICROSECONDS_PER_SECOND              1'000'000

#define MILLISECONDS_PER_CENTISECOND                10
#define MILLISECONDS_PER_DECISECOND                100
#define MILLISECONDS_PER_SECOND                  1'000

#define CENTISECONDS_PER_DECISECOND                 10
#define CENTISECONDS_PER_SECOND                    100

#define DECISECONDS_PER_SECOND                      10

#define SECONDS_PER_MINUTE                          60
#define MINUTES_PER_HOUR                            60
#define HOURS_PER_DAY                               24

#define SECONDS_PER_HOUR                            (SECONDS_PER_MINUTE * MINUTES_PER_HOUR       )
#define SECONDS_PER_DAY                             (SECONDS_PER_HOUR * HOURS_PER_DAY            )

#define DECISECONDS_PER_MINUTE                      (DECISECONDS_PER_SECOND * SECONDS_PER_MINUTE )
#define DECISECONDS_PER_HOUR                        (DECISECONDS_PER_MINUTE * MINUTES_PER_HOUR   )
#define DECISECONDS_PER_DAY                         (DECISECONDS_PER_HOUR * HOURS_PER_DAY        )

#define MILLISECONDS_PER_MINUTE                     (MILLISECONDS_PER_SECOND * SECONDS_PER_MINUTE)
#define MILLISECONDS_PER_HOUR                       (MILLISECONDS_PER_MINUTE * MINUTES_PER_HOUR  )
#define MILLISECONDS_PER_DAY                        (MILLISECONDS_PER_HOUR * HOURS_PER_DAY       )

constexpr uint64_t MICROSECONDS_PER_MINUTE          { static_cast<uint64_t>(MICROSECONDS_PER_SECOND) * SECONDS_PER_MINUTE };
constexpr uint64_t MICROSECONDS_PER_HOUR            { MICROSECONDS_PER_MINUTE * MINUTES_PER_HOUR };
constexpr uint64_t MICROSECONDS_PER_DAY             { MICROSECONDS_PER_HOUR * HOURS_PER_DAY      };

constexpr uint64_t NANOSECONDS_PER_MINUTE           { static_cast<uint64_t>(NANOSECONDS_PER_SECOND) * SECONDS_PER_MINUTE };
constexpr uint64_t NANOSECONDS_PER_HOUR             { NANOSECONDS_PER_MINUTE * MINUTES_PER_HOUR  };
constexpr uint64_t NANOSECONDS_PER_DAY              { NANOSECONDS_PER_HOUR * HOURS_PER_DAY       };

constexpr uint64_t _100_NANOSECONDS_PER_SECOND      { NANOSECONDS_PER_SECOND / 100                     };
constexpr uint64_t _100_NANOSECONDS_PER_MINUTE      { _100_NANOSECONDS_PER_SECOND * SECONDS_PER_MINUTE };
constexpr uint64_t _100_NANOSECONDS_PER_HOUR        { _100_NANOSECONDS_PER_MINUTE * MINUTES_PER_HOUR   };
constexpr uint64_t _100_NANOSECONDS_PER_DAY         { _100_NANOSECONDS_PER_HOUR * HOURS_PER_DAY        };

/*

#define MINUTE_in_seconds                (SECONDS_PER_MINUTE)
#define HOUR_in_seconds                  (MINUTE_in_seconds * MINUTES_PER_HOUR)
#define DAY_in_seconds                   (HOUR_in_seconds * HOURS_PER_DAY)

#define SECOND_in_deciseconds            (DECISECONDS_PER_SECOND)
#define MINUTE_in_deciseconds            (SECOND_in_deciseconds * SECONDS_PER_MINUTE)
#define HOUR_in_deciseconds              (MINUTE_in_deciseconds * MINUTES_PER_HOUR)
#define DAY_in_deciseconds               (HOUR_in_deciseconds * HOURS_PER_DAY)

#define SECOND_in_milliseconds           (MILLISECONDS_PER_SECOND)
#define MINUTE_in_milliseconds           (SECOND_in_milliseconds * SECONDS_PER_MINUTE)
#define HOUR_in_milliseconds             (MINUTE_in_milliseconds * MINUTES_PER_HOUR)
#define DAY_in_milliseconds              (HOUR_in_milliseconds * HOURS_PER_DAY)

#define SECOND_in_microseconds              (MICROSECONDS_PER_SECOND)
constexpr uint64_t MINUTE_in_microseconds   { SECOND_in_microseconds * SECONDS_PER_MINUTE    };
constexpr uint64_t HOUR_in_microseconds     { MINUTE_in_microseconds * MINUTES_PER_HOUR      };
constexpr uint64_t DAY_in_microseconds      { HOUR_in_microseconds * HOURS_PER_DAY           };

constexpr uint64_t SECOND_in_nanoseconds    { NANOSECONDS_PER_SECOND                         };
constexpr uint64_t MINUTE_in_nanoseconds    { SECOND_in_nanoseconds * SECONDS_PER_MINUTE     };
constexpr uint64_t HOUR_in_nanoseconds      { MINUTE_in_nanoseconds * MINUTES_PER_HOUR       };
constexpr uint64_t DAY_in_nanoseconds       { HOUR_in_nanoseconds * HOURS_PER_DAY            };

constexpr uint64_t SECOND_in_100_nanoseconds{ NANOSECONDS_PER_SECOND / 100                   };
constexpr uint64_t MINUTE_in_100_nanoseconds{ SECOND_in_100_nanoseconds * SECONDS_PER_MINUTE };
constexpr uint64_t HOUR_in_100_nanoseconds  { MINUTE_in_100_nanoseconds * MINUTES_PER_HOUR   };
constexpr uint64_t DAY_in_100_nanoseconds   { HOUR_in_100_nanoseconds * HOURS_PER_DAY        };
*/

// clang-format on
