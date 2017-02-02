#ifndef CIRCULAR_NUMBER_H_INCLUDED
#define CIRCULAR_NUMBER_H_INCLUDED

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
/// it can never get that high (because it resets to "min").
///
/// By default, min is set to 0, and max is set to 1. You can change this
/// by calling circular_number.min and circular_number.max respectively,
/// and min will always be the smaller of the two. However, if min == max,
/// t
///
/// Lastly, because of the nature of this data structure, you can not
/// multiply or divide it.


#include <iostream>

template <typename T>
class circular_number
{
protected:
    T value;//stores the number's value

    //Safely set the number's value to something within bounds
    T setWithinBounds(T new_value)
    {
        if (min > max)
        {
            T temp = min
        }

        if (new_value >= max)
        {
            while (new_value >= max)
                new_value = new_value - max + min;
        }
        else if (new_value < min)
        {
            while (new_value < min)
                new_value = new_value + max - min;
        }

        return new_value;
    };


public:
    T max;  //Upper limit of number
    T min;  //Lowest the number can be

    //Default constructor
    circular_number() {max = 1; min = 0; value = 0;};

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
        T new_value = value + arg;//assign new value,

        value = setWithinBounds(new_value);//and set it within bounds.

        return value;
    }

    //When we use the -= operator
    const T& operator-=(const T& arg)
    {
        T new_value = value - arg;//assign new value,

        value = setWithinBounds(new_value);//and set it within bounds.

        return value;
    }


    ///
    ///Addition operators

    //Now for the + operator, if you're adding two circular_number objects.
    friend const T& operator+(const circular_number& arg1, const circular_number& arg2)
    {
        return (arg1.value + arg2.value);
    }

    //the + operator if you're adding a number of type T on the right.
    friend const T& operator+(const circular_number& arg1, const T& arg2)
    {
        return (arg1.value + arg2);
    }

    //the + operator if you're adding a number of type T on the left.
    friend const T& operator+(const T& arg1, const circular_number& arg2)
    {
        return (arg2.value + arg1);
    }

    //and the unary + operator
    const T& operator++()
    {
        return (value + 1);
    }


    ///
    ///Subtraction operators

    //The - operator, if you're subtracting two circular_number objects.
    friend T operator-(const circular_number& arg1, const circular_number& arg2)
    {
        return (arg1.value - arg2.value);
    }

    //the - operator if you're subtracting a number of type T on the right.
    friend T operator-(const circular_number& arg1, const T& arg2)
    {
        return (arg1.value - arg2);
    }

    //the - operator if you're subtracting a number of type T on the left.
    friend T operator-(const T& arg1, const circular_number& arg2)
    {
        return (arg1 - arg2.value);
    }

    //and the unary - operator
    T operator-()
    {
        return (-value);
    }


    //Used for printing the value to the console, or anything else that uses operator<<
    friend std::ostream& operator<< (std::ostream& stream, const circular_number& object)
    {
        stream << object.value; //add value to the stream

        return stream;  //and return.
    }

};



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
