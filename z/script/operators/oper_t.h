/**
 * File:            oper_t.h
 * Namespace:       z::script::oper
 * Description:     Parent class for all script operators.
 *                  Note that operators are only designed to work
 *                  using script values, NOT variables.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Feb. 2017
**/

#pragma once
#ifndef OPER_T_H_INCLUDED
#define OPER_T_H_INCLUDED

#include <stdint.h>

#include "../../core/string.h"

#include "../script_errors.h"
#include "../../core/dynamicStack.h"

namespace z
{
    namespace script
    {
        namespace oper
        {
            typedef uint8_t oper_priority;

            template <typename CHAR>
            class oper_t
            {
            private:
                core::string<CHAR> _string;
                oper_priority oper_pri;

                bool is_unary;

                bool unary_pos_is_front;

            public:
                ///Constructor
                template <typename CHAR_T>
                oper_t(core::string<CHAR_T> oper_string, oper_priority _priority,
                       bool unary_operator, bool operator_before_operand = true)
                {
                    _string = oper_string;

                    oper_pri = _priority;

                    is_unary = unary_operator;

                    unary_pos_is_front = operator_before_operand;
                }


                ///Overload this function when creating a new operator
                virtual error_flag operate(core::dynamic_stack< core::string<CHAR> >& _stack) const
                {
                    return error::INVALID_OPERATION;
                }



                const core::string<CHAR>& str() const {return _string;}

                oper_priority priority() const {return oper_pri;}


                ///These functions are for sorting operators
                bool operator==(const oper_t& other) const
                {
                    return (_string == other._string);
                }

                bool operator>(const oper_t& other) const
                {
                    return (_string > other._string);
                }

                bool operator<(const oper_t& other) const
                {
                    return (_string < other._string);
                }


                bool unary() const {return is_unary;}
                bool binary() const {return !is_unary;}

                bool before() const {return unary_pos_is_front;}
                bool after() const {return !unary_pos_is_front;}
            };
        }
    }
}

#endif // OPER_T_H_INCLUDED
