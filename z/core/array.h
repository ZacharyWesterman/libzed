/**
 * File:            array.h
 * Namespace:       z::core
 *
 * Description:     A wrapper for std::vector.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Feb. 2017
**/

#pragma once
#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED

#include <vector>

namespace z
{
    namespace core
    {
        //The array class is a wrapper for the std::vector class
        //that adds ease of use. Additionally, if I ever decide to
        //stop using vectors, I won't have to change all of my code,
        //just this class.
        template <typename T>
        class array
        {
        protected:
            std::vector<T> array_data;

            void init(T arg1)
            {
                array_data.push_back(arg1);
            }

            template <typename... Args>
            void init(T arg1, Args... args)
            {
                array_data.push_back(arg1);

                init(args...);
            }

        public:
            array() {}
            array(const array&);

            array(T arg1)
            {
                init(arg1);
            }

            template <typename... Args>
            array(T arg1, Args... args)
            {
                init(arg1, args...);
            }

            ~array() {}

            void clear();

            virtual int add(const T&);
            bool insert(const T&, int);
            bool remove(int);

            int size() const;

            T& at(const int);
            const T& at(const int) const;
            T& operator[](const int);
            const T& operator[](const int) const;

            ///Function to check if a given object is in the array
            //linear search (array is unsorted)
            //returns -1 if the object was not found,
            //returns the object's index if it was found
            virtual int find(const T& object) const
            {
                for (int i=0; i<(int)array_data.size(); i++)
                    if (array_data.at(i) == object)
                        return i;

                return -1;
            }

            const array& operator=(const array& other)
            {
                clear();

                for (int i=0; i<(int)other.array_data.size(); i++)
                    array_data.push_back(other.array_data.at(i));

                return *this;
            }

            bool operator==(const array& other) const
            {
                if (array_data.size() != other.array_data.size())
                    return false;

                for (int i=0; i<(int)array_data.size(); i++)
                    if (array_data.at(i) != other.array_data.at(i))
                        return false;

                return true;
            }

            bool is_valid(int position) const
            {
                return ((position < (int)array_data.size()) &&
                        (position >= 0));
            }
        };

        ///Copy constructor
        template <typename T>
        array<T>::array(const array<T>& other)
        {
            for (int i=0; i<(int)other.array_data.size(); i++)
                array_data.push_back(other.array_data.at(i));
        }


        ///Function to empty the array
        template <typename T>
        void array<T>::clear()
        {
            array_data.clear();
        }


        ///Function to append an object to the end of the array
        template <typename T>
        int array<T>::add(const T& object)
        {
            array_data.push_back(object);

            return ((int)array_data.size() - 1);
        }


        ///Function to insert an object to the given index in the array
        //places the given object in that index(if valid), returning false if invalid index
        template <typename T>
        bool array<T>::insert(const T& object, int index)
        {
            //if invalid index, return false
            if (index >= (int)array_data.size())
                return false;

            array_data.insert(array_data.begin() + index, object);

            return true;
        }


        ///Function to remove an object from the array
        //removes an object from the given index, returning false if invalid index
        template <typename T>
        bool array<T>::remove(int index)
        {
            if ((index < 0) || (index >= (int)array_data.size()))
                return false;

            array_data.erase(array_data.begin() + index);

            return true;
        }


        ///Return the number of objects in the array
        template <typename T>
        int array<T>::size() const
        {
            return (int)array_data.size();
        }

        ///Functions to get an object from the array, given an index
        //throw an exception if given an invalid index
        template <typename T>
        T& array<T>::at(int index)
        {
            if ((index >= (int)array_data.size()) ||
                (index < 0))
            {
                throw std::bad_alloc();
            }
            else
            {
                return array_data.at(index);
            }
        }

        template <typename T>
        const T& array<T>::at(int index) const
        {
            if ((index >= (int)array_data.size()) ||
                (index < 0))
            {
                throw std::bad_alloc();
            }
            else
            {
                return array_data.at(index);
            }
        }


        template <typename T>
        T& array<T>::operator[](int index)
        {
            if ((index >= (int)array_data.size()) ||
                (index < 0))
            {
                throw std::bad_alloc();
            }
            else
            {
                return array_data.at(index);
            }
        }

        template <typename T>
        const T& array<T>::operator[](int index) const
        {
            if ((index >= (int)array_data.size()) ||
                (index < 0))
            {
                throw std::bad_alloc();
            }
            else
            {
                return array_data.at(index);
            }
        }
    }
}

#endif // ARRAY_H_INCLUDED
