#include <exception>

namespace z {
namespace util {
/**
 * @brief Exception to throw when attempting a numeric operation on a non-number.
 */
struct nonNumber : public std::exception {
	/**
	 * @brief Exception description.
	 * @return A text description of the exception.
	 */
	const char *what() const throw() {
		return "Attempted numeric operation on non-number";
	}
};
} // namespace util
} // namespace z
