// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <chrono>
#include <vector>

// This is deliberately not a virtual class
class Stopwatch final
{
public:
    explicit Stopwatch(double baseTime = 0.0);
    ~Stopwatch() = default;

    void clear();
    void reset();
    void start();
    void restart();
    double end();
    double getSplit() const;
    double calculate(bool empty = false);

    void   setBaseTime(double baseTime)             { _baseTime = baseTime; }
    double getBaseTime() const                      { return _baseTime; }
    double getElapsedSeconds() const                { return _elapsedSeconds; }
    size_t getNumTimes() const                      { return _times.size(); }

    class Scoped final
    {
    public:
        explicit Scoped(Stopwatch* pStopwatch);
        Scoped(const Scoped&) = delete;
        void operator=(const Scoped&) = delete;
        ~Scoped();

        void end() noexcept;

    private:
        Stopwatch* _pStopwatch;
    };

private:
    std::chrono::high_resolution_clock::time_point _start;
    std::vector<double> _times;
    double _baseTime = 0.0;
    double _elapsedSeconds = 0.0;

    static constexpr double MinSeconds = 0.000001;
};
