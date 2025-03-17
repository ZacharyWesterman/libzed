#pragma once

#include "../core/generator.hpp"

namespace z {
namespace util {

/**
 * @brief Chains two generators together.
 *
 * This function takes two generators and chains them together, so that
 * the second generator is only called when the first generator is exhausted.
 *
 * @param first The first generator.
 * @param second The second generator.
 */
template <typename T, typename U, typename V>
core::generator<T, bool> chain(core::generator<T, U> &first, core::generator<T, V> &second) noexcept {
	return core::generator<T, bool>(false, [&first, &second](bool &first_exhausted) {
		if (!first_exhausted) {
			auto item = first.next();
			if (item.done) {
				first_exhausted = true;
			} else {
				return item;
			}
		}

		return second.next();
	});
}

} // namespace util
} // namespace z
