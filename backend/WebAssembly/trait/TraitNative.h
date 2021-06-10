
#pragma once
#include "../WasmHelper.h"

namespace script {

namespace wasm_backend {

class WasmEngine;

struct ArgumentsData {
  int thiz_;
  int argsBase_;
  int argsCount_;
  WasmEngine* engine_;
};

struct WasmScriptClassState {
  WasmEngine* scriptEngine_ = nullptr;
  Weak<Object> weakRef_;
};

}  // namespace wasm_backend

template <>
struct internal::ImplType<::script::Arguments> {
  using type = wasm_backend::ArgumentsData;
};

template <>
struct internal::ImplType<::script::ScriptClass> {
  using type = wasm_backend::WasmScriptClassState;
};

}  // namespace script
