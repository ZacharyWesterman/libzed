/**
 * File:            array.h
 * Namespace:       z::core
 *
 * Description:     A wrapper for std::vector.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   27 Jul. 2017
**/

#pragma once
#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED

#include <vector>

#include "stream.h"

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
            virtual void add(const array&);
            bool insert(const T&, int);
            bool remove(int);

            bool replace(int, int, const T&);
            bool replace(int, int, const array<T>&);


            array subset(int, int);

            inline int size() const;

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

            bool operator>(const array& other) const
            {
                if (array_data.size() != other.array_data.size())
                    return (array_data.size() > other.array_data.size());

                int gt_count = 0;

                for (int i=0; i<(int)array_data.size(); i++)
                {
                    if (array_data.at(i) > other.array_data.at(i))
                        gt_count++;
                    else if (array_data.at(i) < other.array_data.at(i))
                        gt_count--;
                }

                return gt_count > 0;
            }

            bool operator<(const array& other) const
            {
                if (array_data.size() != other.array_data.size())
                    return (array_data.size() < other.array_data.size());

                int gt_count = 0;

                for (int i=0; i<(int)array_data.size(); i++)
                {
                    if (array_data.at(i) > other.array_data.at(i))
                        gt_count++;
                    else if (array_data.at(i) < other.array_data.at(i))
                        gt_count--;
                }

                return gt_count < 0;
            }

            inline bool operator>=(const array& other) const
            { return !operator<(other); }

            inline bool operator<=(const array& other) const
            { return !operator>(other); }


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
        //returns where the object was placed
        template <typename T>
        int array<T>::add(const T& object)
        {
            array_data.push_back(object);

            return ((int)array_data.size() - 1);
        }

        ///Function to append contents of another array to the end of this array
        template <typename T>
        void array<T>::add(const array& other)
        {
            for (int i=0; i<other.size(); i++)
                array_data.push_back(other[i]);
        }


        ///Function to insert an object to the given index in the array
        //places the given object in that index(if valid), returning false if invalid index
        template <typename T>
        bool array<T>::insert(const T& object, int index)
        {
            //if invalid index, return false
            if (index > (int)array_data.size())
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
        inline int array<T>::size() const
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


        template <typename T>
        bool array<T>::replace(int start, int stop, const T& object)
        {
            if (stop >= (int)array_data.size())
                stop = (int)array_data.size() - 1;

            if (start < 0)
                start = 0;

            if (stop < start)
            {
                return false;
            }
            else
            {
                array_data.erase(array_data.begin() + start, array_data.begin() + stop + 1);
                array_data.insert(array_data.begin() + start, object);

                return true;
            }
        }


        template <typename T>
        bool array<T>::replace(int start, int stop, const array<T>& other)
        {
            if (stop >= (int)array_data.size())
                stop = (int)array_data.size() - 1;

            if (start < 0)
                start = 0;

            if (stop < start)
            {
                return false;
            }
            else
            {
                array_data.erase(array_data.begin() + start, array_data.begin() + stop + 1);

                for (int i=other.size()-1; i>=0; i--)
                    array_data.insert(array_data.begin() + start, other[i]);

                return true;
            }
        }


        template <typename T>
        array<T> array<T>::subset(int start, int stop)
        {
            if (stop >= (int)array_data.size())
                stop = (int)array_data.size() - 1;

            if (start < 0)
                start = 0;


            array<T> output;

            if (stop >= start)
            {
                for (int i=start; i<=stop; i++)
                    output.array_data.push_back(array_data[i]);
            }

            return output;
        }



        ///Stream output template (from array)
        template <typename CHAR, typename T>
        stream<CHAR>& operator<<(stream<CHAR>& arg1, const array<T>& arg2)
        {
            arg1.shift_in();

            for (int i=0; i<arg2.size(); i++)
                arg1 << arg2[i];

            arg1.shift_out();

            return arg1;
        }

        ///Stream input template (to array)
        template <typename CHAR, typename T>
        stream<CHAR>& operator>>(stream<CHAR>& arg1, array<T>& arg2)
        {
            stream<CHAR> data = arg1.pop();

            while(!data.isEmpty())
            {
                T item;
                data >> item;

                arg2.add(item);
            }

            return arg1;
        }
    }
}

#endif // ARRAY_H_INCLUDED
