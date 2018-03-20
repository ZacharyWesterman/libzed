#pragma once
#ifndef REFARRAY_H_INCLUDED
#define REFARRAY_H_INCLUDED

#include <type_traits>

#include "array.h"
#include <z/int.h>

namespace z
{
    namespace core
    {
        /**
         * \brief An extension of the core::array class which
         * assumes all elements to be pointers.
         *
         * \see array
         * \see sortedArray
         */
        template <typename T>
        class refArray : public array<T>
        {
            //only allow this class to work with pointers
            static_assert(std::is_pointer<T>::value,
                          "Template must be of pointer type.");

        private:
            inline void init(const T arg1)
            {
                this->array_data.push_back(arg1);
            }

            template <typename... Args>
            inline void init(const T arg1, const Args... args)
            {
                this->array_data.push_back(arg1);

                init(args...);
            }

        public:
            ///Default constructor.
            refArray() {}

            refArray(const array<T>&);
            refArray(const T);

            template <typename... Args>
            refArray(const T arg1, const Args... args);

            virtual Int find(const T& object) const;
        };


        ///Copy constructor
        template <typename T>
        refArray<T>::refArray(const array<T>& other)
        {
            this->array_data = other.array_data;
        }

        /**
         * \brief Constructor with one argument.
         *
         * Constructs the array with one
         * element already contained.<BR>
         *
         * \b Syntax: array<T> X (arg1); array<T> X = arg1;
         *
         * \param arg1 initializing data.
         */
        template <typename T>
        refArray<T>::refArray(const T arg1)
        {
            this->array_data.push_back(arg1);
        }


        /**
         * \brief List-initialized constructor.
         *
         * Constructs the array with an arbitrary
         * number of elements already contained.<BR>
         *
         * \b Syntax: array<T> X {arg1, arg2, ...};
         * array<T> X = {arg1, arg2, ...};
         *
         * \param arg1 initializing data.
         * \param args cont. initializing data.
         */
        template <typename T>
        template <typename... Args>
        refArray<T>::refArray(const T arg1, const Args... args)
        {
            init(arg1, args...);
        }


        /**
         * \brief Check if a given object with identical
         * data is in the array.
         *
         * Locates the desired index using a linear search,
         * as the array is expected to be unsorted.
         *
         * \param object a pointer to the object to search for.
         *
         * \return The first index that the object data was found at.
         * \b -1 if it was not found.
         */
        template <typename T>
        Int refArray<T>::find(const T& object) const
        {
            for (Int i=0; i<((Int)this->array_data.size()); i++)
                if (*(this->array_data.at(i)) == *object)
                    return i;

            return -1;
        }
    }
}

#endif // REFARRAY_H_INCLUDED
