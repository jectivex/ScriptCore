
#pragma once

#include "../../src/types.h"

namespace script {

namespace lua_backend {
class LuaEngine;
}

template <>
struct internal::ImplType<ScriptEngine> {
  using type = lua_backend::LuaEngine;
};

}  // namespace script
