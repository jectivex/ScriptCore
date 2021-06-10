
#pragma once

#include "../WasmHelper.h"

namespace script {

namespace wasm_backend {

class WasmEngineScope {
  int stackTop_;

 public:
  explicit WasmEngineScope(WasmEngine &);

  ~WasmEngineScope() {
    // restore stack top
    wasm_backend::Stack::setTop(stackTop_);
  }
};

class WasmExitEngineScope {
 public:
  explicit WasmExitEngineScope(WasmEngine &) {
    // exit current entered engine
  }

  ~WasmExitEngineScope() {
    // reenter engine;
  }
};

class WasmStackFrameScope {
  int stackTop_;
  bool hasReturnValue_ = false;

 public:
  explicit WasmStackFrameScope(WasmEngine &) : stackTop_(wasm_backend::Stack::top()) {
    // reserved for return value
    wasm_backend::Stack::pushNull();
  }

  ~WasmStackFrameScope() {
    if (hasReturnValue_) {
      // preserve ths return value
      wasm_backend::Stack::setTop(stackTop_ + 1);
    } else {
      wasm_backend::Stack::setTop(stackTop_);
    }
  }

  template <typename T>
  Local<T> returnValue(const Local<T> &localRef);
};
}  // namespace wasm_backend

template <>
struct internal::ImplType<EngineScope> {
  using type = wasm_backend::WasmEngineScope;
};

template <>
struct internal::ImplType<ExitEngineScope> {
  using type = wasm_backend::WasmExitEngineScope;
};

template <>
struct internal::ImplType<StackFrameScope> {
  using type = wasm_backend::WasmStackFrameScope;
};

}  // namespace script
