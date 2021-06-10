
#pragma once

#include "../../src/Native.h"
#include "V8Engine.hpp"

namespace script {

template <typename T>
ScriptClass::ScriptClass(const ScriptClass::ConstructFromCpp<T>) : internalState_() {
  auto v8Engine = v8_backend::currentEngine();
  auto symbol = v8Engine->constructorMarkSymbol_.Get(v8Engine->isolate_);
  auto pointer = v8::External::New(v8Engine->isolate_, this);

  auto obj = v8Engine->newNativeClass<T>(v8_backend::V8Engine::make<Local<Value>>(symbol),
                                         v8_backend::V8Engine::make<Local<Value>>(pointer));
  internalState_.scriptEngine_ = v8Engine;
  internalState_.weakRef_.Reset(v8Engine->isolate_,
                                v8_backend::V8Engine::toV8(v8Engine->isolate_, obj));
  internalState_.weakRef_.SetWeak();
}

template <typename T>
T *ScriptClass::getScriptEngineAs() const {
  return script::internal::scriptDynamicCast<T *>(internalState_.scriptEngine_);
}

template <typename T>
T *Arguments::engineAs() const {
  return script::internal::scriptDynamicCast<T *>(callbackInfo_.first);
}

}  // namespace script
