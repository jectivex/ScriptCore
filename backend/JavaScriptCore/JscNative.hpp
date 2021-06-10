
#pragma once

#include "JscEngine.hpp"

namespace script {

template <typename T>
ScriptClass::ScriptClass(ScriptClass::ConstructFromCpp<T>) : internalState_() {
  auto jscEngine = jsc_backend::currentEngine();
  auto symbol = jscEngine->constructorMarkSymbol_.get();
  auto thiz = jsc_backend::JscEngine::make<Local<Value>>(
      JSObjectMake(jscEngine->context_, jsc_backend::JscEngine::externalClass_, this));

  auto obj = jscEngine->newNativeClass<T>(symbol, thiz);

  internalState_.scriptEngine_ = jscEngine;
  jsc_backend::JscWeakRef(jsc_interop::toJsc(jscEngine->context_, obj))
      .swap(internalState_.weakRef_);
}

template <typename T>
T *ScriptClass::getScriptEngineAs() const {
  return script::internal::scriptDynamicCast<T *>(internalState_.scriptEngine_);
}

template <typename T>
T *Arguments::engineAs() const {
  return script::internal::scriptDynamicCast<T *>(callbackInfo_.engine);
}

}  // namespace script
