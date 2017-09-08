#pragma once
#ifndef SORTEDARRAY_H_INCLUDED
#define SORTEDARRAY_H_INCLUDED

#include "array.h"

namespace z
{
    namespace core
    {
        /**
         * \brief An extension of the array class
         * which attempts to keep all data sorted.
         *
         * \see array
         * \see refArray
         * \see sortedRefArray
         */
        template <typename T>
        class sortedArray : public array<T>
        {
        private:
            inline void init(const T& arg1)
            {
                add(arg1);
            }

            template <typename... Args>
            inline void init(const T& arg1, const Args&... args)
            {
                add(arg1);

                init(args...);
            }

        public:
            ///Default constructor
            sortedArray() {}

            sortedArray(const T&);

            template <typename... Args>
            sortedArray(const T& arg1, const Args&... args);

            virtual int add(const T&);
            virtual void add(const array<T>&);

            virtual int find(const T&) const;

            int findInsert(const T&) const;

            virtual void sort();
        };


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
        sortedArray<T>::sortedArray(const T& arg1)
        {
            this->array_data.push_back(arg1);
        }

        /**
         * \brief List-initialized constructor.
         *
         * Constructs the array with an arbitrary
         * number of elements already contained and
         * sorted.<BR>
         *
         * \b Syntax: array<T> X {arg1, arg2, ...};
         * array<T> X = {arg1, arg2, ...};
         *
         * \param arg1 initializing data.
         * \param args cont. initializing data.
         */
        template <typename T>
        template <typename... Args>
        sortedArray<T>::sortedArray(const T& arg1, const Args&... args)
        {
            init(arg1, args...);
        }


        /**
         * \brief Add an object to the array.
         *
         * Finds an appropriate location and inserts the
         * object into the array, such that the array
         * remains sorted.
         *
         * \param object the data to add to the array.
         *
         * \return The index where the inserted object now resides.
         */
        template <typename T>
        int sortedArray<T>::add(const T& object)
        {
            int index = findInsert(object);

            this->insert(object, index);

            return index;
        }

        /**
         * \brief Add the contents of another array to this one.
         *
         * For each of the elements in the other array,
         * this function finds an appropriate location in this
         * array and inserts the object, such that this array
         * remains sorted.
         *
         * \param other the array to copy from.
         */
        template <typename T>
        void sortedArray<T>::add(const array<T>& other)
        {
            for (int i=0; i<other.size(); i++)
            {
                int index = findInsert(other[i]);

                this->insert(other[i], index);
            }
        }

        /**
         * \brief Check if a given object is in the array.
         *
         * Locates the desired index using a binary search,
         * as the array is expected to be sorted.
         *
         * \param object the object to search for.
         *
         * \return The first index that the object was found at.
         * \b -1 if it was not found.
         */
        template <typename T>
        int sortedArray<T>::find(const T& object) const
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

        /**
         * \brief Find an index where the given object
         * can be inserted while keeping the array sorted.
         *
         * Locates the desired index using a binary search,
         * as the array is expected to be sorted.
         *
         * \param object the object to search for.
         *
         * \return The first index where the object can be
         * inserted.
         */
        template <typename T>
        int sortedArray<T>::findInsert(const T& object) const
        {
            if (this->array_data.size() == 0)
                return 0;

            int left = 0;
            int right = this->array_data.size()-1;

            while (left < right)
            {
                int center = (left + right) / 2;

                if (this->array_data.at(center) < object)
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

            if (this->array_data.at(left) < object)
                return left+1;
            else
                return left;
        }

        /**
         * \brief Sorts the array.
         *
         * This function currently uses bubble-sort. As
         * such, it is <B>very slow</B> for large arrays!
         * I intend to implement a faster version later.
         */
        template <typename T>
        void sortedArray<T>::sort()
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

#endif // SORTEDARRAY_H_INCLUDED
