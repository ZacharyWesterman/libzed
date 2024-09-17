#include "timeout.hpp"

namespace z {
namespace core {
timeout::timeout(unsigned int timeout_micros, bool run) noexcept : timer(run) {
	timeout_goal = timeout_micros;
}

void timeout::setTimeOut(unsigned int timeout_micros) noexcept {
	timeout_goal = timeout_micros;
}

bool timeout::timedOut() const noexcept {
	return this->micros() >= timeout_goal;
}
} // namespace core
} // namespace z
