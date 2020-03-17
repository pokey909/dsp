//
// Created by Alexander Lenhardt on 16.03.20.
//

#ifndef DSP_TYPES_HPP
#define DSP_TYPES_HPP

#include <cstdint>
#include <type_traits>

template<class T>
struct Sample {
    T stereoData[2];

    Sample() {
        stereoData[0] = 0;
        stereoData[1] = 0;
    }
    Sample(const Sample<T> &other) {
        stereoData[0] = other.stereoData[0];
        stereoData[1] = other.stereoData[1];
    }
    Sample<T>& operator=(const T& b) {
        stereoData[0] = b;
        stereoData[1] = b;
        return *this;
    }
    Sample<T>& operator=(const Sample<T>& b) {
        if (&b != this) {
            stereoData[0] = b.stereoData[0];
            stereoData[1] = b.stereoData[1];
        }
        return *this;
    }
};

template<class T, std::enable_if_t<std::is_arithmetic<T>::value, int> = 0>
Sample<T> operator*(Sample<T> a, float b) {
    a.stereoData[0] *= b;
    a.stereoData[1] *= b;
    return a;
}

template<class T, class B, std::enable_if_t<std::is_integral<B>::value, int> = 0>
Sample<T> operator+(Sample<T> a, B b) {
    a.stereoData[0] += b;
    a.stereoData[1] += b;
    return a;
}

template<class T, class B, std::enable_if_t<std::is_arithmetic<B>::value, int> = 0>
Sample<T>& operator+=(Sample<T> &a, B b) {
    a.stereoData[0] += b;
    a.stereoData[1] += b;
    return a;
}


template<class T>
Sample<T>& operator+=(Sample<T> &a, const Sample<T> &b) {
    a.stereoData[0] += b.stereoData[0];
    a.stereoData[1] += b.stereoData[1];
    return a;
}

using SampleI16 = Sample<int16_t>;
using SampleF32 = Sample<float>;

#endif //DSP_TYPES_HPP
