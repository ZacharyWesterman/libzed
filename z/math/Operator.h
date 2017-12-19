#pragma once
#ifndef OPERATOR_H_INCLUDED
#define OPERATOR_H_INCLUDED

namespace z
{
    namespace math
    {
        class Operator
        {
        protected:
            bool _error;
        public:
            Operator() {_error = false;}

            template <typename T>
            T operate(const T& arg1, const T& arg2) const
            {return T(arg1);}

            inline bool error() {return _error;}
        };


        class equals : public Operator
        {
        public:
            template <typename T>
            T operate(const T& arg1, const T& arg2) const
            {
                return (arg1 == arg2);
            }
        };
    }
}

#endif // OPERATOR_H_INCLUDED
