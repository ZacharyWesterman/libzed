#pragma once
#ifndef TIMEOUT_H_INCLUDED
#define TIMEOUT_H_INCLUDED

#include "timer.h"

namespace z
{
    namespace core
    {
        /**
         * \brief A class for easily managing timing.
         *
         * An extended version of the timer class
         * which includes a timeout boundary.
         */
        class timeout : public timer
        {
        private:
            Float timeout_goal;

        public:
            /**
             * \brief Timeout constructor.
             *
             * \param timeout_micros the number of \b microseconds
             * after which the timer will time out. If this is set
             * to a negative value, then it will never time out.
             *
             * \param run an optional parameter to decide
             * whether the timer should start now.
             */
            inline timeout(Float timeout_micros = -1, bool run = true) : timer(run)
            {
                timeout_goal = timeout_micros;
            }

            /**
             * \brief Set the time out goal.
             *
             * \param timeout_micros the number of \b microseconds
             * after which the timer will time out. If this is set
             * to a negative value, then it will never time out.
             */
            inline void setTimeOut(Float timeout_micros)
            {
                timeout_goal = timeout_micros;
            }

            /**
             * \brief Check whether the timer has timed out.
             *
             * \return \b True if the timer has passed the specified
             * time limit. \b False otherwise.
             */
            inline bool timedOut() const
            {
                return ((timeout_goal >= 0) && (this->micros() >= timeout_goal));
            }
        };
    }
}

#endif // TIMEOUT_H_INCLUDED
