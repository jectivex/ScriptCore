
#pragma once

#include "../../src/Native.h"
#include "QjsEngine.h"

namespace script {

template <typename T>
ScriptClass::ScriptClass(const ScriptClass::ConstructFromCpp<T>) : internalState_() {
  auto& engine = qjs_backend::currentEngine();
  internalState_.engine = &engine;

  auto pointer = JS_NewObjectClass(engine.context_, qjs_backend::QjsEngine::kPointerClassId);
  qjs_backend::checkException(pointer);
  JS_SetOpaque(pointer, this);

  auto ret = engine.template newNativeClass<T>({qjs_interop::makeLocal<Value>(pointer)});
  auto ref = qjs_interop::getLocal(ret);

  internalState_.weakRef_ = ref;

  // schedule -> JS_Free(ref)
  engine.extendLifeTimeToNextLoop(ref);
}

template <typename T>
T* Arguments::engineAs() const {
  return script::internal::scriptDynamicCast<T*>(callbackInfo_.engine_);
}

}  // namespace script
