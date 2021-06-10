
#pragma once

#include <lua.hpp>
#include "../../src/types.h"
#include "../../src/utils/GlobalWeakBookkeeping.hpp"

namespace script::lua_backend {
class LuaEngine;
}

namespace script::internal {

template <typename T>
struct ImplType<Local<T>> {
  using type = int;
};

template <typename T>
struct ImplType<Global<T>> {
  struct type {
    lua_backend::LuaEngine* engine = nullptr;
    lua_Integer index = 0;
    GlobalWeakBookkeeping::HandleType handle{};

    type() = default;

    type(const type& copy) : engine(copy.engine), index(copy.index) {}
    type(type&& move) noexcept : engine(move.engine), index(move.index) {
      move.index = 0;
      move.engine = nullptr;
    }

    type& operator=(const type& copy) {
      index = copy.index;
      engine = copy.engine;
      return *this;
    }

    type& operator=(type&& copy) noexcept {
      index = copy.index;
      engine = copy.engine;

      copy.index = 0;
      copy.engine = nullptr;
    }
  };
};

template <typename T>
struct ImplType<Weak<T>> {
  using type = typename ImplType<Global<T>>::type;
};

}  // namespace script::internal
