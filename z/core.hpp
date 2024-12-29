#pragma once
/**
 * @file z/core.hpp
 * @namespace z::core
 * @brief Section containing core functions and classes.
 *
 * This section adds core functionality and is
 * required as it is often referenced by other parts of the library.
 * It can be used by itself, however, as it does not
 * require any libraries besides STL.
 **/

/// Strings
#include "core/string.hpp"

/// Arrays
#include "core/array.hpp"
#include "core/circularBuffer.hpp"
#include "core/refArray.hpp"
#include "core/sortedArray.hpp"
#include "core/sortedRefArray.hpp"

/// Timers
#include "core/timeout.hpp"
#include "core/timer.hpp"

/// Miscellaneous
#include "core/charFunctions.hpp"
#include "core/compare.hpp"
#include "core/join.hpp"
#include "core/sizable.hpp"
#include "core/split.hpp"

/// Generators
#include "core/generator.hpp"
#include "core/range.hpp"

/// Memoization
#include "core/memoize.hpp"
