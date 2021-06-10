
#pragma once

#include "../../src/types.h"

namespace script {
struct lua_interop;

template <>
struct internal::ImplType<StringHolder> {
  struct type {
    const char* string = nullptr;
    size_t len = 0;
  };
};

template <>
struct internal::ImplType<internal::interop> {
  using type = lua_interop;
};

}  // namespace script
