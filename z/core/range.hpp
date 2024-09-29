#pragma once

#include "generator.hpp"

namespace z {
namespace core {

generator<long, long> range(long begin, long end, long step = 1) noexcept;

generator<long, long> range(long end) noexcept;

} // namespace core
} // namespace z