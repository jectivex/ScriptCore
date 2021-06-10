
#pragma once

#include <new>

#define SCRIPTCORE_DISALLOW_COPY(CLASS_NAME) \
  CLASS_NAME(const CLASS_NAME&) = delete; \
  CLASS_NAME& operator=(const CLASS_NAME&) = delete

#define SCRIPTCORE_DISALLOW_MOVE(CLASS_NAME) \
  CLASS_NAME(CLASS_NAME&&) = delete;      \
  CLASS_NAME& operator=(CLASS_NAME&&) = delete

#define SCRIPTCORE_DISALLOW_COPY_AND_MOVE(CLASS_NAME) \
  SCRIPTCORE_DISALLOW_COPY(CLASS_NAME);               \
  SCRIPTCORE_DISALLOW_MOVE(CLASS_NAME)

#define SCRIPTCORE_DISALLOW_NEW()                                            \
  static void* operator new(std::size_t) = delete;                        \
  static void* operator new(std::size_t, const std::nothrow_t&) = delete; \
  static void* operator new[](std::size_t) = delete;                      \
  static void* operator new[](std::size_t, const std::nothrow_t&) = delete

#define SCRIPTCORE_UNUSED(val) static_cast<void>(val)

namespace script::internal {

template <typename T>
struct ImplType {
  // using type = <the implemented type>;
};

}  // namespace script::internal

// SCRIPTCORE_BACKEND_TRAIT_PREFIX is defined in CMake
#define SCRIPTCORE_MARCO_TO_STRING(x) SCRIPTCORE_MARCO_TO_STRING_INNER(x)
#define SCRIPTCORE_MARCO_TO_STRING_INNER(x) #x
#define SCRIPTCORE_MARCO_JOIN(x, y) SCRIPTCORE_MARCO_JOIN_INNER(x, y)
#define SCRIPTCORE_MARCO_JOIN_INNER(x, y) x##y
#define SCRIPTCORE_BACKEND(FILE) \
  SCRIPTCORE_MARCO_TO_STRING(SCRIPTCORE_MARCO_JOIN(SCRIPTCORE_BACKEND_TRAIT_PREFIX, FILE))

#ifdef _MSC_VER

// MSVC only support the standart _Pragma on recent version, use the extension key word here
#define SCRIPTCORE_BEGIN_INCLUDE_LIBRARY __pragma(warning(push, 0))
#define SCRIPTCORE_END_INCLUDE_LIBRARY __pragma(pop)

#elif defined(__clang__)

#define SCRIPTCORE_BEGIN_INCLUDE_LIBRARY \
  _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Wall\"")

#define SCRIPTCORE_END_INCLUDE_LIBRARY _Pragma("clang diagnostic pop")

#elif defined(__GNUC__)
// GCC can't suppress all warnings by -Wall
// suppress anything encountered explicitly
// 1. -Wcast-function-type for QuickJs

#define SCRIPTCORE_BEGIN_INCLUDE_LIBRARY                                        \
  _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wall\"") \
      _Pragma("GCC diagnostic ignored \"-Wcast-function-type\"")

#define SCRIPTCORE_END_INCLUDE_LIBRARY _Pragma("GCC diagnostic pop")

#else

// disable warnings from library header
#define SCRIPTCORE_BEGIN_INCLUDE_LIBRARY
#define SCRIPTCORE_END_INCLUDE_LIBRARY

#endif
