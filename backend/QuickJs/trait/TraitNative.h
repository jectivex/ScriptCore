
#pragma once
#include "../QjsHelper.h"

namespace script {

namespace qjs_backend {

struct ArgumentsData {
  QjsEngine* engine_;
  JSValueConst thiz_;
  size_t argc_;
  JSValueConst* argv_;
};

struct QjsScriptClassState {
  QjsEngine* engine = nullptr;
  JSValueConst weakRef_ = JS_UNDEFINED;
};

}  // namespace qjs_backend

template <>
struct internal::ImplType<::script::Arguments> {
  using type = qjs_backend::ArgumentsData;
};

template <>
struct internal::ImplType<::script::ScriptClass> {
  using type = qjs_backend::QjsScriptClassState;
};

}  // namespace script
