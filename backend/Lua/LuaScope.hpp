
#pragma once

#include "../../src/Scope.h"
#include "LuaHelper.hpp"

namespace script::lua_backend {

template <typename T>
Local<T> StackFrameScopeImpl::returnValue(const Local<T> &localRef) {
  return LuaEngine::make<Local<T>>(returnRawValue(LuaEngine::localRefIndex(localRef)));
}

}  // namespace script::lua_backend
