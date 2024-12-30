#include "sentinel.hpp"

namespace z {
namespace core {

const sentinel infinity([](long, long) { return false; });

} // namespace core
} // namespace z