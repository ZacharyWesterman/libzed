/**
 * File:            timeout.h
 * Namespace:       z::core
 * Description:     Extended version of the timer class
 *                  which includes a timeout boundary.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   21 Apr. 2017
**/

#pragma once
#ifndef TIMEOUT_H_INCLUDED
#define TIMEOUT_H_INCLUDED

#include "timer.h"

namespace z
{
    namespace core
    {
        class timeout : public timer
        {
        private:
            long timeout_goal;

        public:
            inline timeout(long timeout_micros, bool run = true) : timer(run)
            {
                timeout_goal = timeout_micros;
            }


            inline void setTimeOut(long timeout_micros)
            {
                timeout_goal = timeout_micros;
            }

            inline bool timedOut() const
            {
                return (this->micros() >= timeout_goal);
            }
        };
    }
}

#endif // TIMEOUT_H_INCLUDED
