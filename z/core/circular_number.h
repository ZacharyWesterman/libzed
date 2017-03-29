/**
 * File:            circular_number.h
 * Namespace:       z::core
 * Description:     An old template that allows you to
 *                  have numbers automatically loop back
 *                  to an arbitrary minimum when they reach
 *                  an arbitrary maximum (like how a clock
 *                  goes from 1->12 then back to 1).
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   1 Mar. 2017
**/


/// This template can be used to allow data to "loop back on itself",
/// for example if you wanted an integer whose max value was 8, this
/// template would allow you create that integer. Adding 1 to it would
/// then reset its value back to 0, attempting to set it to 9 would
/// instead set it to 1, etc.
///
/// This is only safe for data types that can be evaluated to a number,
/// so using this template with std::string might not be the best idea.
///
/// ALSO, unlike "min", "max" is the upper LIMIT for the number, meaning
/// the number can never get that high (although it should be noted that,
/// when printing the value, it may appear to reach the max value. This
/// is due to rounding errors with floating-point values, e.g. 0.1).
///
/// By default, min is set to 0, and max is set to 1. You can change this
/// by calling circular_number.min and circular_number.max respectively,
/// and min will always be the smaller of the two.


#pragma once
#ifndef CIRCULAR_NUMBER_H_INCLUDED
#define CIRCULAR_NUMBER_H_INCLUDED

#include <iostream>
using namespace std;

namespace z
{
    namespace core
    {
        template <typename T>
        class circular_number
        {
        protected:
            T value;//stores the number's value

            int last_overflow; //stores the number of times the last operation made the number loop

            //Safely set the number's value to something within bounds
            T setWithinBounds(T new_value)
            {
                last_overflow = 0;

                if (min > max)
                {
                    T temp = min;

                    min = max;
                    max = temp;
                }

                while (new_value >= max)
                {
                    new_value = new_value - (max - min);
                    last_overflow++;
                }

                while (new_value < min)
                {
                    new_value = new_value + (max - min);
                    last_overflow--;
                }


                return new_value;
            };


        public:
            T max;  //Upper limit of number
            T min;  //Lowest the number can be

            //Default constructor
            circular_number(T max = 1, T min = 0, T value = 0)
            {
                if (min > max)
                {
                    this->max = min;
                    this->min = max;
                }
                else
                {
                    this->max = max;
                    this->min = min;
                }


                if ((value < min) || (value >= max))
                    this->value = min;
                else
                    this->value = value;

            }

            //returns number's current value
            T val() {return value;}

            ///
            ///Assignment operators

            //When we use the = operator.
            const T& operator=(const T& arg)
            {
                value = setWithinBounds(arg);//make sure the value is within bounds, then set it equal.

                return value;
            }

            //When we use the += operator
            const T& operator+=(const T& arg)
            {
                value = setWithinBounds(value + arg);

                return value;
            }

            //When we use the -= operator
            const T& operator-=(const T& arg)
            {
                value = setWithinBounds(value - arg);

                return value;
            }


            ///
            ///Addition operators

            //Now for the + operator, if you're adding two circular_number objects.
            const T operator+(const circular_number& arg2)
            {
                return setWithinBounds(value + arg2.value);
            }

            //the + operator if you're adding a number of type T on the right.
            const T operator+(const T& arg2)
            {
                return setWithinBounds(value + arg2);
            }

            //the + operator if you're adding a number of type T on the left.
            friend const T operator+(const T& arg1, const circular_number& arg2)
            {
                return setWithinBounds(arg2.value + arg1);
            }

            //and the unary + operator
            const T operator++()
            {
                value = setWithinBounds(value + 1);

                return value;
            }


            ///
            ///Subtraction operators

            //The - operator, if you're subtracting two circular_number objects.
            const T operator-(const circular_number& arg2)
            {
                return setWithinBounds(value - arg2.value);
            }

            //the - operator if you're subtracting a number of type T on the right.
            const T operator-(const T& arg2)
            {
                return setWithinBounds(value - arg2);
            }

            //the - operator if you're subtracting a number of type T on the left.
            friend T operator-(const T& arg1, const circular_number& arg2)
            {
                return setWithinBounds(arg1.value - arg2.value);
            }

            //and the unary - operator
            T operator-()
            {
                return setWithinBounds(-value);
            }


            //how many times the last operation made the number loop
            //if positive, there was an overflow
            //if negative, there was an underflow
            const int overflow()
            {
                return last_overflow;
            }
        };
    }
}


#endif // CIRCULAR_NUMBER_H_INCLUDED


/// A little example of how to use the code.
/// Just copy and paste into your C++ project.
/// It increments the number's value and prints it out.
/*

#include <iostream>
using namespace std;
int main()
{

    circular_number<int>(input);

    input.max = 11;
    input.min = 5;

    for (int i=0; i<20; i++)
    {
        input = i;

        cout << input;
        cout << endl;
    }
}

*/
