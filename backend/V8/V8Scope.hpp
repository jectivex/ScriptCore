
#pragma once

#include "V8Engine.h"
#include "V8Scope.h"

namespace script::v8_backend {

template <typename T>
Local<T> V8HandleScope::returnValue(const Local<T>& localRef) {
  return v8_backend::V8Engine::make<Local<T>>(
      handleScope_.Escape(v8_backend::V8Engine::toV8(handleScope_.GetIsolate(), localRef)));
}

}  // namespace script::v8_backend
