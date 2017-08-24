/**
 * File:            zeta.h
 * Namespace:       z::math
 *
 * Description:     Constexpr function for approximating the
 *                  reimann-zeta sum.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   21 Aug. 2017
**/

#pragma once
#ifndef ZETA_H_INCLUDED
#define ZETA_H_INCLUDED

namespace z
{
    namespace math
    {
        //compute zeta(s) with highest possible precision
        zFloat zeta(const int s)
        {
            zFloat result = 1.0;

            int i = 2;
            zFloat z_denom = 0.5;

            while ( (z_denom + result) > result )
            {
                result += z_denom;

                i++;
                int power = 1;
                for (int j=0; j<s; j++)
                    power *= i;

                z_denom = 1.0 / (zFloat)power;
            }

            return result;
        }
    }
}

#endif // ZETA_H_INCLUDED
