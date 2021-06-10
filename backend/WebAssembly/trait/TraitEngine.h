
#pragma once

#include <string>
#include "../../../src/types.h"

namespace script {

namespace wasm_backend {
class WasmEngine;
}

template <>
struct internal::ImplType<ScriptEngine> {
  using type = wasm_backend::WasmEngine;
};

}  // namespace script
