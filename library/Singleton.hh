#pragma once

#include <stdlib.h>
#include <new>
#include <type_traits>

using namespace std;

template<typename T>
class Singleton
{
protected:
    Singleton() noexcept = default;

    Singleton(const Singleton&) = delete;

    Singleton& operator=(const Singleton&) = delete;

    virtual ~Singleton() = default; // to silence base class Singleton<T> has a
    // non-virtual destructor [-Weffc++]

public:
    static T& getInstance() noexcept(std::is_nothrow_constructible<T>::value)
    {
        // Guaranteed to be destroyed.
        // Instantiated on first use.
        // Thread safe in C++11
        static T instance{};

        return instance;
    }
};
