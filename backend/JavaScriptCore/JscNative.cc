
#include "JscNative.hpp"
#include "../../src/Native.hpp"
#include "JscEngine.hpp"
#include "JscReference.hpp"

namespace script {

Arguments::Arguments(jsc_backend::ArgumentsData callbackInfo) : callbackInfo_(callbackInfo) {}

Arguments::~Arguments() = default;

Local<Object> Arguments::thiz() const {
  return jsc_backend::JscEngine::make<Local<Value>>(callbackInfo_.thisObject).asObject();
}

bool Arguments::hasThiz() const { return callbackInfo_.thisObject != nullptr; }

size_t Arguments::size() const { return callbackInfo_.size; }

Local<Value> Arguments::operator[](size_t i) const {
  if (i >= size()) {
    return {};
  }
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  return jsc_backend::JscEngine::make<Local<Value>>(callbackInfo_.arguments[i]);
}

ScriptEngine* Arguments::engine() const { return callbackInfo_.engine; }

ScriptClass::ScriptClass(const script::Local<script::Object>& scriptObject) : internalState_() {
  auto engine = &jsc_backend::currentEngineChecked();
  internalState_.scriptEngine_ = engine;
  jsc_backend::JscWeakRef(jsc_interop::toJsc(engine->context_, scriptObject))
      .swap(internalState_.weakRef_);
}

Local<Object> ScriptClass::getScriptObject() const {
  return jsc_interop::makeLocal<Value>(internalState_.weakRef_.get()).asObject();
}

Local<Array> ScriptClass::getInternalStore() const {
  auto engine = getScriptEngineAs<jsc_backend::JscEngine>();
  return engine->getInternalStorageBySymbolFunction_.get()
      .call({}, getScriptObject(), engine->internalStorageSymbol_.get())
      .asArray();
}

ScriptEngine* ScriptClass::getScriptEngine() const { return internalState_.scriptEngine_; }

ScriptClass::~ScriptClass() {
  auto engine = internalState_.scriptEngine_;
  internalState_.weakRef_.reset(engine);
}

}  // namespace script
