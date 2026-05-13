/**
 * @file templates.hpp
 * @brief Utility template definitions to allow for simpler type restrictions.
 */

#include <iterator>
#include <type_traits>

namespace z {
namespace core {

/**
 * @brief The type of the dereferenced value from an iterable.
 * This is used to determine the type of value that the generator will std::optional.
 * @tparam T
 */
template <typename T>
using dereference = std::remove_const_t<std::remove_reference_t<decltype(*std::declval<decltype(std::declval<T>().begin())>())>>;

/**
 * @brief The type of the value that an iterator generates.
 * This is used to determine the type of iterator that the generator will use.
 * @tparam T
 */
template <typename T>
using iterator_value = std::remove_const_t<decltype(std::declval<T>().begin())>;

/**
 * @brief The type of a const iterator from a const reference to a container.
 * This is used when iterating over a const reference to ensure proper const_iterator deduction.
 * @tparam T
 */
template <typename T>
using const_iterator_value = decltype(std::declval<const T&>().begin());

/**
 * @brief The default specialization for types that are not iterators.
 * @tparam T
 */
template <typename T, typename = void>
struct is_iterator : std::false_type {};

/**
 * @brief Template specialization for types that are iterators.
 * @tparam T
 */
template <typename T>
struct is_iterator<T, std::void_t<decltype(std::begin(std::declval<T>()))>> : std::true_type {};

} // namespace core
} // namespace z
