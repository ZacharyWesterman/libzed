/**
 * File:            sorted_array.h
 * Namespace:       z::core
 * Description:     An extension of the array class which
 *                  attempts to keep all data sorted.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   12 Feb. 2017
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
            virtual int add(const T&);

            virtual int find(const T&) const;

            virtual void sort();
        };



        template <typename T>
        int sorted_array<T>::add(const T& object)
        {
            if (this->array_data.size() == 0)
            {
                this->array_data.push_back(object);
                return 0;
            }
            else
            {
                int left = 0;
                int right = this->array_data.size()-1;


                while (left < right)
                {
                    int center = (left + right) / 2;


                    if (this->array_data.at(center) > object)
                    {
                        right = center;
                    }
                    else if (this->array_data.at(center) < object)
                    {
                        left = center + 1;
                    }
                    else
                    {
                        this->insert(object, center);
                        return center;
                    }


                    //std::cout << left << ":" << right << std::endl;
                }


                if (this->array_data.at(left) < object)
                    this->insert(object, left+1);
                else
                    this->insert(object, left);

                return left;
            }
        }


        template <typename T>
        int sorted_array<T>::find(const T& object) const
        {
            if (this->array_data.size() == 0)
                return -1;

            int left = 0;
            int right = this->array_data.size()-1;

            while (left < right)
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


        template <typename T>
        void sorted_array<T>::sort()
        {
            bool done = true;

            do
            {
                done = true;


                for (int i=0; i<(int)this->array_data.size()-1; i++)
                {
                    if (this->array_data[i] > this->array_data[i+1])
                    {
                        done = false;

                        T temp = this->array_data[i];
                        this->array_data[i] = this->array_data[i+1];
                        this->array_data[i+1] = temp;
                    }
                }
            }
            while (!done);
        }
    }
}

#endif // SORTED_ARRAY_H_INCLUDED
