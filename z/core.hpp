#pragma once
/**
 * \file z/core.hpp
 * \namespace z::core
 * \brief Section containing core functions and classes.
 *
 * This section adds core functionality and is
 * required as it is often referenced by other parts of the library.
 * It can be used by itself, however, as it does not
 * require any libraries besides STL.
**/

///Strings
#include "core/string.hpp"

///Arrays
#include "core/array.hpp"
#include "core/sortedArray.hpp"
#include "core/refArray.hpp"
#include "core/sortedRefArray.hpp"
#include "core/circularBuffer.hpp"

///Timers
#include "core/timer.hpp"
#include "core/timeout.hpp"

///Miscellaneous
#include "core/charFunctions.hpp"
#include "core/sizable.hpp"
#include "core/split.hpp"
#include "core/join.hpp"
#include "core/compare.hpp"

///Streams
#include "core/stream.hpp"
#include "core/binaryStream.hpp"
#include "core/memoryStream.hpp"
#include "core/stringStream.hpp"
