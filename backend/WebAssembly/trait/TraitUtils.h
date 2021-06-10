
#pragma once

#include "../../../src/types.h"

namespace script {

struct wasm_interop;

namespace wasm_backend {

struct WasmStringHolder {
  mutable const char* cString_;
  mutable size_t length_;
  const int index_;
};

}  // namespace wasm_backend

template <>
struct internal::ImplType<StringHolder> {
  using type = wasm_backend::WasmStringHolder;
};

template <>
struct internal::ImplType<internal::interop> {
  using type = wasm_interop;
};

}  // namespace script
