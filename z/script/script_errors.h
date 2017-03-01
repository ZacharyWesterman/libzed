/**
 * File:            script_errors.h
 * Namespace:       z::script::error
 * Description:     The z::script::error_flag type is defined here.
 *                  Additionally, all error flags are defined here.
 *                  Error flags are all multiples of 2, so you can
 *                  potentially return every error in a single value.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Feb. 2017
**/

#pragma once
#ifndef SCRIPT_ERRORS_H_INCLUDED
#define SCRIPT_ERRORS_H_INCLUDED


namespace z
{
    namespace script
    {
        typedef uint32_t error_flag;

        namespace error
        {
            enum ERRORS
            {
                NONE =              0x00000000,
                DIV_BY_ZERO =       0x00000001,
                INVALID_CHARACTER = 0x00000002,
                MISSING_OPERAND =   0x00000004,
                INVALID_OPERATION = 0x00000008,
                MISSING_L_PARENTH = 0x00000010,
                MISSING_R_PARENTH = 0x00000020,
                TOO_FEW_PARAMS =    0x00000040,
                TOO_MANY_PARAMS =   0x00000080,
                INVALID_PARAM =     0x00000100,
                NON_REAL_NUMBER =   0x00000200,
                OUT_OF_BOUNDS =     0x00000400,
                UNKNOWN_VARIABLE =  0x00000800,
                WRONG_VAR_TYPE =    0x00001000,
                BAD_FORMATTING =    0x00002000,
                MISSING_OPERATOR =  0x00004000,
                UNKNOWN_COMMAND =   0x00008000,
                BAD_REDEFINITION =  0x00010000,
                NO_ENTRY_POINT =    0x00020000,
                BAD_SCOPING =       0x00040000,
                UNEXP_COMMAND =     0x00080000,
                UNEXP_END_OF_SCRIPT=0x00100000,
                UNKNOWN_LABEL =     0x00200000,
                UNKNOWN_SUBROUTINE =0x00400000
            };
        }
    }
}

#endif // SCRIPT_ERRORS_H_INCLUDED
