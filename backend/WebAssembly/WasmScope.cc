
#include "WasmScope.hpp"
#include <sstream>

namespace script::wasm_backend {

WasmEngineScope::WasmEngineScope(WasmEngine &) : stackTop_(-1) {
#ifdef __EMSCRIPTEN_PTHREADS__
  if (wasm_backend::WasmEngine::engineThreadId_ != std::this_thread::get_id()) {
    std::ostringstream oss;
    oss << "WasmEngine can only be used in it's creation thread!"
        << " Current thread is:[" << std::this_thread::get_id() << "]"
        << " Creation thread is:[" << wasm_backend::WasmEngine::engineThreadId_ << "]";
    throw std::runtime_error(oss.str());
  }
#endif
  stackTop_ = wasm_backend::Stack::top();
}

}  // namespace script::wasm_backend
