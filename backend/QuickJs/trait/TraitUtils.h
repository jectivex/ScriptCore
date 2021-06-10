
#pragma once
#include "../QjsHelper.h"

namespace script {

struct qjs_interop;

namespace qjs_backend {

struct StringHolderImpl {
  JSContext* context_;
  const char* string_;
  size_t length_;
};

}  // namespace qjs_backend

template <>
struct internal::ImplType<StringHolder> {
  using type = qjs_backend::StringHolderImpl;
};

template <>
struct internal::ImplType<internal::interop> {
  using type = qjs_interop;
};

}  // namespace script
