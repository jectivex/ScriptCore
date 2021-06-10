
#pragma once

#include <cstdint>
#include <string>
#include "../../src/types.h"

namespace script::internal {

template <typename T>
struct ImplType<Local<T>> {
  using type = int;
};

template <typename T>
struct ImplType<Global<T>> {
  using type = int;
};

template <typename T>
struct ImplType<Weak<T>> {
  using type = int;
};

}  // namespace script::internal
