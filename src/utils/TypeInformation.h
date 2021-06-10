
#pragma once

#ifdef __cpp_rtti
#include <typeindex>
#else
#include <type_traits>
#include <utility>
#endif

namespace script::internal {

#ifdef __cpp_rtti

using TypeIndex = std::type_index;

template <typename T>
TypeIndex typeIndexOf() {
  return std::type_index(typeid(T));
}

template <typename R, typename T>
R scriptDynamicCast(T&& t) {
  return dynamic_cast<R>(std::forward<T>(t));
}

#else

using TypeIndex = const void*;

namespace detail {

template <typename T>
TypeIndex typeIndexOfInner() {
  struct Helper {};
  // we only need to make sure:
  // for each type it returns different pointer
  static Helper index;
  return &index;
}

}  // namespace detail

template <typename T>
TypeIndex typeIndexOf() {
  return detail::typeIndexOfInner<std::remove_cv_t<T> >();
}

template <typename R, typename T>
R scriptDynamicCast(T&& t) {
  return static_cast<R>(std::forward<T>(t));
}

#endif

}  // namespace script::internal
