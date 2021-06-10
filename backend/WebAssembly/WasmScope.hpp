
#pragma once
#include "../../src/Scope.h"
#include "WasmEngine.h"

namespace script::wasm_backend {

template <typename T>
Local<T> WasmStackFrameScope::returnValue(const Local<T> &localRef) {
  hasReturnValue_ = true;
  return WasmEngine::make<Local<T>>(wasm_backend::Stack::set(stackTop_ + 1, localRef.asValue()));
}

}  // namespace script::wasm_backend
