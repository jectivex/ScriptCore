
#pragma once

namespace script {

namespace template_backend {

struct ArgumentsData {
  int stackBase;
  size_t size;
};

struct JscScriptClassState {
  ScriptEngine* scriptEngine_ = nullptr;
  Weak<Object> weakRef_;
};

}  // namespace template_backend

template <>
struct internal::ImplType<::script::Arguments> {
  using type = template_backend::ArgumentsData;
};

template <>
struct internal::ImplType<::script::ScriptClass> {
  using type = template_backend::JscScriptClassState;
};

}  // namespace script
