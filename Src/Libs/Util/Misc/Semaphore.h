#pragma once

#include <mutex>

template <typename Mutex, typename CondVar>
class basic_semaphore
{
public:
    using native_handle_type = typename CondVar::native_handle_type;

    explicit basic_semaphore(size_t count = 0);
    basic_semaphore(const basic_semaphore&) = delete;
    basic_semaphore(basic_semaphore&&) = delete;
    basic_semaphore& operator=(const basic_semaphore&) = delete;
    basic_semaphore& operator=(basic_semaphore&&) = delete;

    void notify();
    void wait();
    bool try_wait();
    template<class Rep, class Period>
    bool wait_for(const std::chrono::duration<Rep, Period>& duration);
    template<class Clock, class Duration>
    bool wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time);

    native_handle_type native_handle();

private:
    Mutex   _mutex;
    CondVar _cv;
    size_t  _count;
};

using semaphore = basic_semaphore<std::mutex, std::condition_variable>;

template <typename Mutex, typename CondVar>
basic_semaphore<Mutex, CondVar>::basic_semaphore(size_t count)
    : _count{ count }
{
}

template <typename Mutex, typename CondVar>
void basic_semaphore<Mutex, CondVar>::notify()
{
    std::lock_guard<Mutex> lock{ _mutex };
    ++_count;
    _cv.notify_one();
}

template <typename Mutex, typename CondVar>
void basic_semaphore<Mutex, CondVar>::wait()
{
    std::unique_lock<Mutex> lock{ _mutex };
    _cv.wait(lock, [&]
    {
        return _count > 0;
    });
    --_count;
}

template <typename Mutex, typename CondVar>
bool basic_semaphore<Mutex, CondVar>::try_wait()
{
    std::lock_guard<Mutex> lock{ _mutex };

    if (_count > 0)
    {
        --_count;
        return true;
    }

    return false;
}

template <typename Mutex, typename CondVar>
template<class Rep, class Period>
bool basic_semaphore<Mutex, CondVar>::wait_for(const std::chrono::duration<Rep, Period>& duration)
{
    std::unique_lock<Mutex> lock{ _mutex };
    auto finished = _cv.wait_for(lock, d, [&]
    {
        return _count > 0;
    });

    if (finished)
    {
        --_count;
    }

    return finished;
}

template <typename Mutex, typename CondVar>
template<class Clock, class Duration>
bool basic_semaphore<Mutex, CondVar>::wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time)
{
    std::unique_lock<Mutex> lock{ _mutex };
    auto finished = _cv.wait_until(lock, timeout_time, [&]
    {
        return _count > 0;
    });

    if (finished)
    {
        --_count;
    }

    return finished;
}

template <typename Mutex, typename CondVar>
typename basic_semaphore<Mutex, CondVar>::native_handle_type basic_semaphore<Mutex, CondVar>::native_handle()
{
    return _cv.native_handle();
}
