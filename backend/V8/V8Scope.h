
#pragma once

#include "../../src/Reference.h"
#include "../../src/types.h"
#include "V8Helper.h"

namespace script::v8_backend {

class V8EngineScope {
  v8::Locker locker_;
  v8::Isolate::Scope isolateScope_;
  v8::HandleScope handleScope_;
  v8::Context::Scope contextScope_;

 public:
  explicit V8EngineScope(V8Engine& engine);

  ~V8EngineScope() = default;
};

class V8ExitEngineScope {
  v8::Unlocker unlocker_;

 public:
  explicit V8ExitEngineScope(V8Engine& engine);
  ~V8ExitEngineScope() = default;
};

class V8HandleScope {
  v8::EscapableHandleScope handleScope_;

 public:
  explicit V8HandleScope(V8Engine& engine);

  ~V8HandleScope() = default;

  template <typename T>
  Local<T> returnValue(const Local<T>& localRef);
};

}  // namespace script::v8_backend
