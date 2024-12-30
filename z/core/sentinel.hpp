#pragma once
#include <functional>

namespace z {
namespace core {

/**
 * @brief A sentinel that stops a numeric generator when the value reaches a certain point.
 */
class sentinel {
	std::function<bool(long, long)> lambda;

public:
	sentinel() = delete;

	/**
	 * @brief Constructor.
	 * @param lambda The lambda function that determines when the generator should stop.
	 */
	sentinel(std::function<bool(long, long)> lambda) : lambda(lambda) {}

	/**
	 * @brief Check if the generator should stop.
	 * @param value The current value of the generator.
	 * @param step The step size of the generator.
	 * @return True if the generator should stop, false otherwise.
	 */
	bool operator()(long value, long step) const noexcept {
		return lambda(value, step);
	};
};

/**
 * @brief A sentinel that tells numeric generators to run forever.
 * @warning Calling generator functions that consume all values (e.g. `count()`) will cause an infinite loop.
 */
extern const sentinel infinity;

} // namespace core
} // namespace z