#include "dirname.hpp"
#include "executable.hpp"

namespace z {
namespace file {
/**
 * @brief Get the directory path of the running executable.
 * @return A string containing the directory of the currently running executable.
 * @threadsafe_function_yes
 */
inline zpath execdir() noexcept {
	return dirname(executable());
}
} // namespace file
} // namespace z