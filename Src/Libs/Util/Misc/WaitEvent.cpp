// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "WaitEvent.h"

#include <chrono>

using namespace std;

void WaitEvent::set()
{
    scoped_lock<mutex> lock(_mutex);
    _isSet = true;
    _cv.notify_all();
}

void WaitEvent::reset()
{
    scoped_lock<mutex> lock(_mutex);
    _isSet = false;
}

void WaitEvent::wait()
{
    unique_lock<mutex> lock(_mutex);
    _cv.wait(lock, [&]()->bool
    {
        return _isSet;
    });
    _isSet = false;
}

bool WaitEvent::waitFor(int64_t timeoutMillseconds)
{
    unique_lock<mutex> lock(_mutex);
    return _cv.wait_for(lock, chrono::milliseconds(timeoutMillseconds), [&]()->bool
    {
        return _isSet;
    });
}