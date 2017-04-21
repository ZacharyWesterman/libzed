/**
 * File:            timer.h
 * Namespace:       z::core
 * Description:     Timer class. Because this class uses the
 *                  chrono library, it requires AT LEAST C++11.
 *
 * Required
 * compiler flag:   -std=c++11
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   21 Apr. 2017
**/

#pragma once
#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <chrono>

namespace z
{
    namespace core
    {
        class timer
        {
            typedef std::chrono::high_resolution_clock high_resolution_clock;
            typedef std::chrono::microseconds _microseconds;
            typedef std::chrono::milliseconds _milliseconds;
            typedef std::chrono::seconds _seconds;

        private:
            high_resolution_clock::time_point start;

        public:
            inline timer (bool run = true)
            {
                if (run)
                    reset();
            }

            inline void reset()
            {
                start = high_resolution_clock::now();
            }


            inline long micros() const
            {
                return (long)(std::chrono::duration_cast<_microseconds>
                        (high_resolution_clock::now() - start)).count();
            }

            inline long millis() const
            {
                return (long)(std::chrono::duration_cast<_milliseconds>
                        (high_resolution_clock::now() - start)).count();
            }

            inline long seconds() const
            {
                return (long)(std::chrono::duration_cast<_seconds>
                        (high_resolution_clock::now() - start)).count();
            }
        };
    }
}

#endif // TIMER_H_INCLUDED
