#pragma once

#include "../core/generator.hpp"

namespace z {
namespace system {
core::generator<bool, long> interval(double ms, long maxTriggers = -1) noexcept;
} // namespace system
} // namespace z
