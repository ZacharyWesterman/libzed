/**
 * File:            core.h
 * Namespace:       none
 * Description:     Header file which includes all
 *                  z::core functions and datatypes.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   2 Aug. 2017
**/


#pragma once
#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED

///Strings
#include "core/string.h"
#include "core/stream.h"

///Arrays
#include "core/array.h"
#include "core/sortedArray.h"
#include "core/sortedRefArray.h"

///Misc data structures
#include "core/dynamicStack.h"
#include "core/circularNumber.h"

///Timers
#include "core/timer.h"
#include "core/timeout.h"

///Misc functions
#include "core/pause.h"

///Dynamic library loading
#include "core/loadLibrary.h"

#endif // CORE_H_INCLUDED
