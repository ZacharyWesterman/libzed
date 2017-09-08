#pragma once
#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED

#include <vector>

#include "stream.h"

namespace z
{
    namespace core
    {
        /**
         * \brief A wrapper for std::vector.
         *
         * This class is a wrapper for the std::vector class
         * that adds ease of use. Additionally, if I ever decide to
         * stop using std::vector, I won't have to change all of my code,
         * just this class.
         */
        template <typename T>
        class array
        {
        protected:
            ///The data in the array.
            std::vector<T> array_data;

        private:
            inline void init(const T& arg1)
            {
                array_data.push_back(arg1);
            }

            template <typename... Args>
            inline void init(const T& arg1, const Args&... args)
            {
                array_data.push_back(arg1);

                init(args...);
            }

        public:
            ///Default constructor.
            array() {}

            array(const array&);

            array(const T&);

            template <typename... Args>
            array(const T& arg1, const Args&... args);

            ///Destructor
            ~array() {}

            inline void clear();

            virtual int add(const T&);
            virtual void add(const array&);
            bool insert(const T&, int);
            bool remove(int);

            void replace(int, int, const T&);
            void replace(int, int, const array<T>&);


            array subset(int, int);

            inline int size() const;

            inline T& at(const int);
            inline const T& at(const int) const;
            inline T& operator[](const int);
            inline const T& operator[](const int) const;


            virtual int find(const T& object) const;

            const array& operator=(const array& other);

            bool operator==(const array& other) const;
            bool operator>(const array& other) const;
            bool operator<(const array& other) const;
            inline bool operator>=(const array& other) const;
            inline bool operator<=(const array& other) const;


            bool is_valid(int position) const;
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
        array<T>::array(const T& arg1)
        {
            array_data.push_back(arg1);
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
        array<T>::array(const T& arg1, const Args&... args)
        {
            init(arg1, args...);
        }

        /**
         * \brief Array assignment operator.
         *
         * Clear the contents of this array and create
         * a copy of another array's contents into this one.
         *
         * \param other the array to copy from.
         *
         * \return This array after the operation (for
         * \b a=b=c type expressions).
         */
        template <typename T>
        const array<T>& array<T>::operator=(const array<T>& other)
        {
            array_data = other.array_data;

            return *this;
        }

        /**
         * \brief Check whether two arrays' contents are the same.
         *
         * \param other the array to compare with this one.
         *
         * \return \b True if both arrays contain the same
         * contents in the same order, and the same number of
         * contents. \b False otherwise.
         */
        template <typename T>
        bool array<T>::operator==(const array<T>& other) const
        {
            if (array_data.size() != other.array_data.size())
                return false;

            for (int i=0; i<(int)array_data.size(); i++)
                if (array_data.at(i) != other.array_data.at(i))
                    return false;

            return true;
        }

        /**
         * \brief Array greater-than operator
         *
         * \param other the array to compare with this one.
         *
         * \return \b True if the difference between the two arrays'
         * elements add to a positive number, or this array has more
         * elements. \b False otherwise.
         */
        template <typename T>
        bool array<T>::operator>(const array<T>& other) const
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

        /**
         * \brief Array less-than operator
         *
         * \param other the array to compare with this one.
         *
         * \return \b True if the difference between the two arrays'
         * elements add to a negative number, or this array has fewer
         * elements. \b False otherwise.
         */
        template <typename T>
        bool array<T>::operator<(const array& other) const
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

        /**
         * \brief Array greater-than-or-equal operator
         *
         * \param other the array to compare with this one.
         *
         * \return \b False if the difference between the two arrays'
         * elements add to a negative number, or this array has fewer
         * elements. \b True otherwise.
         */
        template <typename T>
        inline bool array<T>::operator>=(const array& other) const
        {
            return !operator<(other);
        }

        /**
         * \brief Array less-than-or-equal operator
         *
         * \param other the array to compare with this one.
         *
         * \return \b False if the difference between the two arrays'
         * elements add to a positive number, or this array has more
         * elements. \b True otherwise.
         */
        template <typename T>
        inline bool array<T>::operator<=(const array<T>& other) const
        {
            return !operator>(other);
        }

        /**
         * \brief Check if a given object is in the array.
         *
         * Locates the desired index using a linear search,
         * as the array is expected to be unsorted.
         *
         * \param object the object to search for.
         *
         * \return The first index that the object was found at.
         * \b -1 if it was not found.
         */
        template <typename T>
        int array<T>::find(const T& object) const
        {
            for (int i=0; i<(int)array_data.size(); i++)
                if (array_data.at(i) == object)
                    return i;

            return -1;
        }

        ///Copy constructor
        template <typename T>
        array<T>::array(const array<T>& other)
        {
            array_data = other.array_data;
        }

        ///Clear the data in the array.
        template <typename T>
        inline void array<T>::clear()
        {
            array_data.clear();
        }


        /**
         * \brief Add an object to the array.
         *
         * Appends the given data to the end of the array.
         *
         * \param object the data to add to the array.
         *
         * \return The index where the inserted object now resides.
         */
        template <typename T>
        int array<T>::add(const T& object)
        {
            array_data.push_back(object);

            return ((int)array_data.size() - 1);
        }

        /**
         * \brief Add another array to this array.
         *
         * Copies the contents of another array and
         * appends them to the end of this array.
         *
         * \param other the array to copy from.
         */
        template <typename T>
        void array<T>::add(const array& other)
        {
            for (int i=0; i<other.size(); i++)
                array_data.push_back(other[i]);
        }


        ///Function to insert an object to the given index in the array
        //places the given object in that index(if valid), returning false if invalid index
        /**
         * \brief Insert an object into the array.
         *
         * Inserts an object into the given index in the array, if possible.
         *
         * \param object the data to add to the array.
         * \param index the index in the array to insert the object.
         *
         * \return The index where the inserted object now resides.
         */
        template <typename T>
        bool array<T>::insert(const T& object, int index)
        {
            //if invalid index, return false
            if (index > (int)array_data.size())
                return false;

            array_data.insert(array_data.begin() + index, object);

            return true;
        }


        /**
         * \brief Remove an object from the array.
         *
         * \param index the index of the object to be removed.
         *
         * \return \b True if the object was successfully removed.
         * \b False otherwise.
         */
        template <typename T>
        bool array<T>::remove(int index)
        {
            if ((index < 0) || (index >= (int)array_data.size()))
                return false;

            array_data.erase(array_data.begin() + index);

            return true;
        }


        /**
         * \brief Get the size of the array.
         *
         * \return The number of objects in the array.
         */
        template <typename T>
        inline int array<T>::size() const
        {
            return (int)array_data.size();
        }

        /**
         * \brief Function to get the object at the given index.
         *
         * \param index the index of the desired object.
         *
         * \return The object at the given index.
         *
         * \see operator[](int)
         */
        template <typename T>
        inline T& array<T>::at(int index)
        {
            return array_data.at(index);
        }

        /**
         * \brief Const function to get the object at the given index.
         *
         * \param index the index of the desired object.
         *
         * \return The object at the given index.
         *
         * \see operator[](int) const
         */
        template <typename T>
        inline const T& array<T>::at(int index) const
        {
           return array_data.at(index);
        }

        /**
         * \brief Function to get the object at the given index.
         *
         * Identical behavior to at(int), but allows indexing
         * with square brackets.
         *
         * \param index the index of the desired object.
         *
         * \return The object at the given index.
         *
         * \see at(int)
         */
        template <typename T>
        inline T& array<T>::operator[](int index)
        {
            return array_data.at(index);
        }

        /**
         * \brief Const function to get the object at the given index.
         *
         * Identical behavior to at(int), but allows indexing
         * with square brackets.
         *
         * \param index the index of the desired object.
         *
         * \return The object at the given index.
         *
         * \see at(int) const
         */
        template <typename T>
        inline const T& array<T>::operator[](int index) const
        {
            return array_data.at(index);
        }


        /**
         * \brief Replace all objects in the given range with an object.
         *
         * Removes all objects from \b start to \b stop (inclusive),
         * and replaces them with the given object.
         *
         * \param start the index of the first object to replace.
         * \param stop the index of the last object to replace.
         * \param object the object to insert into the gap.
         *
         * \see replace(int,int,const array&)
         */
        template <typename T>
        void array<T>::replace(int start, int stop, const T& object)
        {
            if (stop >= (int)array_data.size())
                stop = (int)array_data.size() - 1;

            if (start < 0)
                start = 0;

            if (stop >= start)
            {
                array_data.erase(array_data.begin() + start, array_data.begin() + stop + 1);
                array_data.insert(array_data.begin() + start, object);
            }
        }

        /**
         * \brief Replace all objects in the given range with an array.
         *
         * Removes all objects from \b start to \b stop (inclusive),
         * and replaces them with the objects in a given array.
         *
         * \param start the index of the first object to replace.
         * \param stop the index of the last object to replace.
         * \param other the array to copy from.
         *
         * \see replace(int,int,const T&)
         */
        template <typename T>
        void array<T>::replace(int start, int stop, const array<T>& other)
        {
            if (stop >= (int)array_data.size())
                stop = (int)array_data.size() - 1;

            if (start < 0)
                start = 0;

            if (stop >= start)
            {
                array_data.erase(array_data.begin() + start, array_data.begin() + stop + 1);

                for (int i=other.size()-1; i>=0; i--)
                    array_data.insert(array_data.begin() + start, other[i]);
            }
        }


        /**
         * \brief Get a contiguous subset of the elements in the array.
         *
         * \param start the index of the first object to copy.
         * \param stop the index of the last object to copy.
         *
         * \return An array containing the objects from \b start to
         * \b stop, inclusive.
         */
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


        /**
         * \brief Check if an index is within the bounds of the array.
         *
         * \param index the index to check.
         *
         * \return \b True if the given index is within array bounds.
         * \b False otherwise.
         */
        template <typename T>
        bool array<T>::is_valid(int index) const
        {
            return ((index < (int)array_data.size()) &&
                    (index >= 0));
        }


        /* DEPRECATED- DELETE OR MOVE TO STREAM CLASS
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
        */
    }
}

#endif // ARRAY_H_INCLUDED
