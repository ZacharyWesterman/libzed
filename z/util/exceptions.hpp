#include <stdexcept>

namespace z {
namespace util {
/**
 * @brief Exception thrown when attempting a numeric operation on a non-number.
 */
struct nonnumeric : public std::runtime_error {
	/**
	 * @brief Constructor.
	 */
	nonnumeric() : std::runtime_error("Attempted numeric operation on non-number") {}
};
} // namespace util
} // namespace z
