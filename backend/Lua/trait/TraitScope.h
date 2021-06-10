
#pragma once

#include <mutex>
#include "../../src/types.h"

namespace script {

namespace lua_backend {

class LuaEngine;

class StackFrameScopeImpl {
 private:
  LuaEngine *engine_ = nullptr;
  int base_ = 0;
  bool hasReturnValue_ = false;

 public:
  explicit StackFrameScopeImpl(LuaEngine &);

  ~StackFrameScopeImpl();

  template <typename T>
  Local<T> returnValue(const Local<T> &localRef);

  int returnRawValue(int index);

  friend class EngineScopeImpl;
};

class EngineScopeImpl {
  LuaEngine *prevEngine_ = nullptr;
  std::lock_guard<std::mutex> lockGuard_;
  StackFrameScopeImpl stack_;

 public:
  explicit EngineScopeImpl(LuaEngine &);

  ~EngineScopeImpl();
};

class ExitEngineScopeImpl {
  LuaEngine *engine_ = nullptr;

 public:
  explicit ExitEngineScopeImpl(LuaEngine &);

  ~ExitEngineScopeImpl();
};

}  // namespace lua_backend

template <>
struct internal::ImplType<EngineScope> {
  using type = lua_backend::EngineScopeImpl;
};

template <>
struct internal::ImplType<ExitEngineScope> {
  using type = lua_backend::ExitEngineScopeImpl;
};

template <>
struct internal::ImplType<StackFrameScope> {
  using type = lua_backend::StackFrameScopeImpl;
};

}  // namespace script
