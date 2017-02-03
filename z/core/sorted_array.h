/**
 * File:            sorted_array.h
 * Namespace:       z::core
 * Description:     An extension of the array class which
 *                  attempts to keep all data sorted.
 *                  Note that this is not finished yet, and
 *                  it does not work.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Feb. 2017
**/


#pragma once
#ifndef SORTED_ARRAY_H_INCLUDED
#define SORTED_ARRAY_H_INCLUDED

#include "./array.h"

namespace z
{
    namespace core
    {
        template <typename T>
        class sorted_array : public array<T>
        {
        private:
            //private

        public:
            int find(const T& object);
        };


        template <typename T>
        int sorted_array<T>::find(const T& object)
        {
            int left = 0;
            int right = this->array_data.size()-1;

            while (left != right)
            {
                int center = (left + right) / 2;

                if ((this->array_data.at(center)) < object)
                {
                    left = center + 1;
                }
                else if (this->array_data.at(center) > object)
                {
                    right = center - 1;
                }
                else
                {
                    return center;
                }
            }

            if (this->array_data.at(left) == object)
                return left;
            else
                return -1;
        }
    }
}

#endif // SORTED_ARRAY_H_INCLUDED
