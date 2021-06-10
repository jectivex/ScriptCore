
#include "../../src/Native.hpp"
#include "WasmEngine.h"
#include "WasmReference.hpp"

namespace script {

Arguments::Arguments(InternalCallbackInfoType callbackInfo) : callbackInfo_(callbackInfo) {}

Arguments::~Arguments() = default;

Local<Object> Arguments::thiz() const {
  if (!hasThiz()) {
    throw Exception("arguments don't have 'this' argument");
  }
  return wasm_backend::WasmEngine::make<Local<Object>>(callbackInfo_.thiz_);
}

bool Arguments::hasThiz() const {
  return wasm_backend::WasmEngine::make<Local<Value>>(callbackInfo_.thiz_).isObject();
}

size_t Arguments::size() const { return static_cast<size_t>(callbackInfo_.argsCount_); }

Local<Value> Arguments::operator[](size_t i) const {
  if (i >= static_cast<size_t>(callbackInfo_.argsCount_)) {
    return {};
  }
  return wasm_backend::WasmEngine::make<Local<Value>>(callbackInfo_.argsBase_ + i);
}

ScriptEngine* Arguments::engine() const { return callbackInfo_.engine_; }

ScriptClass::ScriptClass(const script::Local<script::Object>& scriptObject) : internalState_() {
  internalState_.scriptEngine_ = &wasm_backend::currentEngine();
  internalState_.weakRef_ = scriptObject;
}

Local<Object> ScriptClass::getScriptObject() const { return internalState_.weakRef_.get(); }

Local<Array> ScriptClass::getInternalStore() const {
  int ret = wasm_backend::NativeHelper::getInternalStore(
      wasm_backend::WasmEngine::refIndex(internalState_.weakRef_));
  if (ret == -1) {
    throw Exception("script object already destroyed");
  }
  return wasm_backend::WasmEngine::make<Local<Array>>(ret);
}

ScriptEngine* ScriptClass::getScriptEngine() const { return internalState_.scriptEngine_; }

ScriptClass::~ScriptClass() = default;
}  // namespace script
