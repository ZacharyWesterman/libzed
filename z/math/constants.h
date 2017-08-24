/**
 * File:            constants.h
 * Namespace:       z::math
 *
 * Description:     Some global mathematical constants.
 *                  IEEE doubles only have ~15 decimal
 *                  place precision, but to avoid
 *                  rounding errors I have made these
 *                  constants as accurate as I
 *                  reasonably can.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Mar. 2017
**/

#pragma once
#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

namespace z
{
    namespace math
    {
        //pi to the 31st decimal place.
        const zFloat pi     = 3.1415926535897932384626433832795;

        //e to the 31st decimal place.
        const zFloat e      = 2.7182818284590452353602874713527;

        //Pythagoras' constant to the 65th decimal place.
        const zFloat sqrt2  = 1.41421356237309504880168872420969807856967187537694807317667973799;

        //The square root of 5 to the 59th decimal place.
        const zFloat sqrt5  = 2.23606797749978969640917366873127623544061835961152572427089;

        //The golden ratio to the 55th decimal place.
        const zFloat phi    = 1.6180339887498948482045868343656381177203091798057628621;
    }
}

#endif // CONSTANTS_H_INCLUDED
