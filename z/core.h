/**
 * \file z/core.h
 * \namespace z::core
 * \brief Library containing core functions and classes.
 *
 * This library adds core functionality and is
 * required as it is often referenced by other libraries.
 * It can be used by itself, however, as it does not
 * require any libraries besides STL.
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

///Timers
#include "core/timer.h"
#include "core/timeout.h"

///Misc functions
#include "core/pause.h"
#include "core/charFunctions.h"

///Dynamic library loading
#include "core/loadLibrary.h"

#endif // CORE_H_INCLUDED
