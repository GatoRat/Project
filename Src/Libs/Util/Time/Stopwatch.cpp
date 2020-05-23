// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Stopwatch.h"
#include <algorithm>

using namespace std;

Stopwatch::Stopwatch(double baseTime)
    : _baseTime(baseTime)
{
    start();
}

void Stopwatch::clear()
{
    _times.clear();
    _elapsedSeconds = 0.0;
}

void Stopwatch::reset()
{
    _times.resize(0);
    _elapsedSeconds = 0.0;
}

void Stopwatch::start()
{
    _start = std::chrono::high_resolution_clock::now();
}

void Stopwatch::restart()
{
    reset();
    start();
}

double Stopwatch::end()
{
    const double elapsed = getSplit();
    _times.push_back(elapsed);
    return elapsed;
}

double Stopwatch::getSplit() const
{
    const std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<double>>(end - _start).count();
}

double Stopwatch::calculate(bool empty)
{
    double elapsed = 0.0;

    if (_times.size() > 1)
    {
        size_t start = 0;
        size_t end = _times.size();

        if (_times.size() > 3)
        {
            sort(_times.begin(), _times.end());

            double factor;
            if (_times.size() >= 30)
            {
                factor = 0.30;
            }
            else if (_times.size() >= 20)
            {
                factor = 0.25;
            }
            else
            {
                factor = 0.20;
            }

            start = static_cast<size_t>((static_cast<double>(_times.size()) * factor) + 0.9999999999);
            end -= start;
            if (end <= start)
            {
                end = start + 1;
            }
        }

        for (size_t i = start; i < end; i++)
        {
            elapsed += _times[i];
        }

        elapsed /= static_cast<double>(end - start);
    }
    else if (!_times.empty())
    {
        elapsed = _times[0];
    }

    if (empty)
    {
        _times.clear();
    }

    _elapsedSeconds = max(elapsed - _baseTime, 0.0);
    if (_elapsedSeconds < MinSeconds)
    {
        _elapsedSeconds = 0.0;
    }

    return _elapsedSeconds;
}

Stopwatch::Scoped::Scoped(Stopwatch* pStopwatch)
    : _pStopwatch(pStopwatch)
{
    if (_pStopwatch)
    {
        _pStopwatch->start();
    }
}

Stopwatch::Scoped::~Scoped()
{
    end();
}

void Stopwatch::Scoped::end() noexcept
{
    if (_pStopwatch)
    {
        _pStopwatch->end();
        _pStopwatch = nullptr;
    }
}
