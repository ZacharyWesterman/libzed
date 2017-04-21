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
        public:
            timeout(double timeout_micros, bool run = true) : timer(run)
            {

            }


        };
    }
}

#endif // TIMEOUT_H_INCLUDED
