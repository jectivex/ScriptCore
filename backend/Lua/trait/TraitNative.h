
#pragma once

#include "../../src/types.h"

namespace script {

namespace lua_backend {
class LuaEngine;

struct ArgumentsData {
  LuaEngine* engine = nullptr;
  int stackBase = 0;
  size_t size = 0;
  bool isInstanceFunc = false;
};

struct LuaScriptClassState {
  LuaEngine* scriptEngine_ = nullptr;
  Weak<Object> weakRef_;
};

}  // namespace lua_backend

template <>
struct internal::ImplType<::script::Arguments> {
  using type = lua_backend::ArgumentsData;
};

template <>
struct internal::ImplType<::script::ScriptClass> {
  using type = lua_backend::LuaScriptClassState;
};

}  // namespace script
