#pragma once
#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <chrono>

#include <z/float.h>

namespace z
{
    namespace core
    {
        /**
         * \brief A class for easily managing timers.
         *
         * Because this class uses the chrono library,
         * it requires <B>at least</B> C++11.
         */
        class timer
        {
            typedef std::chrono::high_resolution_clock high_resolution_clock;
            typedef std::chrono::microseconds _microseconds;
            typedef std::chrono::milliseconds _milliseconds;
            typedef std::chrono::seconds _seconds;
            typedef std::chrono::minutes _minutes;
            typedef std::chrono::hours _hours;

        private:
            high_resolution_clock::time_point start;

        public:

            /**
             * \brief Timer constructor.
             *
             * \param run an optional parameter to decide
             * whether the timer should start now.
             */
            inline timer (bool run = true)
            {
                if (run)
                    reset();
            }

            /**
             * \brief Reset the timer.
             */
            inline void reset()
            {
                start = high_resolution_clock::now();
            }

            /**
             * \brief Get the elapsed time in \b microseconds.
             *
             * \return The number of \b microseconds that have passed
             * since the timer was last reset.
             */
            inline Float micros() const
            {
                return (Float)(std::chrono::duration_cast<_microseconds>
                        (high_resolution_clock::now() - start)).count();
            }

            /**
             * \brief Get the elapsed time in \b milliseconds.
             *
             * \return The number of \b milliseconds that have passed
             * since the timer was last reset.
             */
            inline Float millis() const
            {
                Float _micro = (Float)(std::chrono::duration_cast<_microseconds>
                        (high_resolution_clock::now() - start)).count();

                return (_micro / 1000);
            }

            /**
             * \brief Get the elapsed time in \b seconds.
             *
             * \return The number of \b seconds that have passed
             * since the timer was last reset.
             */
            inline Float seconds() const
            {
                Float _micro = (Float)(std::chrono::duration_cast<_microseconds>
                        (high_resolution_clock::now() - start)).count();

                return (_micro / 1000000);
            }

            /**
             * \brief Get the elapsed time in \b minutes.
             *
             * \return The number of \b minutes that have passed
             * since the timer was last reset.
             */
            inline Float minutes() const
            {
                Float _micro = (Float)(std::chrono::duration_cast<_microseconds>
                        (high_resolution_clock::now() - start)).count();

                return (_micro / 60000000);
            }

            /**
             * \brief Get the elapsed time in \b minutes.
             *
             * \return The number of \b minutes that have passed
             * since the timer was last reset.
             */
            inline Float hours() const
            {
                Float _micro = (Float)(std::chrono::duration_cast<_microseconds>
                        (high_resolution_clock::now() - start)).count();

                return (_micro / 3600000000);
            }
        };
    }
}

#endif // TIMER_H_INCLUDED
