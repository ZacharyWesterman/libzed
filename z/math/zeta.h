/**
 * File:            zeta.h
 * Namespace:       z::math
 *
 * Description:     Constexpr function for approximating the
 *                  reimann-zeta sum. Note that the
 *                  approximation is less accurate for
 *                  single-precision floats.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   24 Aug. 2017
**/

#pragma once
#ifndef ZETA_H_INCLUDED
#define ZETA_H_INCLUDED

namespace z
{
    namespace math
    {
        //compute zeta(s) with highest possible precision
        ///for zeta(s), it is assumed that s>1
        zFloat zeta(const int s)
        {
            zFloat result = 1.0;
            zFloat oldResult = 0.0;

            int i = 2;

            while (result != oldResult)
            {
                oldResult = result;

                zFloat denomP = 1.0 / (zFloat)i;

                zFloat partial = 1;
                for (int j=0; j<s; j++)
                    partial *= denomP;

                result += partial;

                i++;
            }

            //std::cout << i << std::endl;

            return result;
        }
    }
}

#endif // ZETA_H_INCLUDED
