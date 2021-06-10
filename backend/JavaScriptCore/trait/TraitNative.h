
#pragma once

#include <JavaScriptCore/JavaScript.h>
#include "../../../src/foundation.h"
#include "../../../src/types.h"
#include "../JscWeakRef.h"

namespace script {

namespace jsc_backend {

class JscEngine;

struct ArgumentsData {
  JscEngine* engine;
  JSValueRef thisObject;
  const JSValueRef* arguments;
  size_t size;
};

struct JscScriptClassState {
  JscEngine* scriptEngine_ = nullptr;
  const void* classDefine = nullptr;
  JscWeakRef weakRef_;
};

}  // namespace jsc_backend

template <>
struct internal::ImplType<::script::Arguments> {
  using type = jsc_backend::ArgumentsData;
};

template <>
struct internal::ImplType<::script::ScriptClass> {
  using type = jsc_backend::JscScriptClassState;
};

}  // namespace script
