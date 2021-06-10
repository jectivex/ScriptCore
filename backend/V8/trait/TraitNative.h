
#pragma once

#include "../../../src/foundation.h"
#include "../../../src/types.h"
#include "../V8Helper.h"

namespace script {

namespace v8_backend {
class V8Engine;
}

template <>
struct internal::ImplType<::script::Arguments> {
  using type = std::pair<v8_backend::V8Engine*, v8::FunctionCallbackInfo<v8::Value>>;
};

namespace v8_backend {

struct V8ScriptClassState {
  V8Engine* scriptEngine_ = nullptr;
  v8::Global<v8::Object> weakRef_;
  void* classDefine_ = nullptr;
};

}  // namespace v8_backend

template <>
struct internal::ImplType<::script::ScriptClass> {
  using type = v8_backend::V8ScriptClassState;
};

}  // namespace script
