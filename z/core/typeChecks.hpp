#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnonnull"

#include <type_traits>

namespace z ::core::types {
/// Internal class to do template check if method exists.
struct no {};

template <typename T, typename Arg>
no operator<(const T &, const Arg &);
template <typename T, typename Arg>
no operator>(const T &, const Arg &);
template <typename T, typename Arg>
no operator==(const T &, const Arg &);

/// A template to enable specializations for an object if `operator<` exists on it.
template <typename T, typename Arg = T>
struct lesserExists {
	enum { value = !std::is_same<decltype(std::declval<T>() < std::declval<Arg>()), no>::value };
};

/// A template to enable specializations for an object if `operator>` exists on it.
template <typename T, typename Arg = T>
struct greaterExists {
	enum { value = !std::is_same<decltype(std::declval<T>() > std::declval<Arg>()), no>::value };
};

/// A template to enable specializations for an object if `operator==` exists on it.
template <typename T, typename Arg = T>
struct equalExists {
	enum { value = !std::is_same<decltype(std::declval<T>() == std::declval<Arg>()), no>::value };
};

} // namespace z::core::types

#pragma GCC diagnostic pop
