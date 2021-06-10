
#pragma once

#include <JavaScriptCore/JavaScript.h>
#include <atomic>
#include "../../src/foundation.h"

namespace script::jsc_backend {

class JscEngine;

class JscWeakRef {
 public:
  JscWeakRef();

  explicit JscWeakRef(JSValueRef value);

  ~JscWeakRef();

  JscWeakRef(const JscWeakRef& copy);

  JscWeakRef& operator=(const JscWeakRef& copy);

  JscWeakRef(JscWeakRef&& move) noexcept;

  JscWeakRef& operator=(JscWeakRef&& move) noexcept;

  void reset(JscEngine* engine);

  void swap(JscWeakRef& rhs) noexcept;

  JSValueRef get() const;

  bool isEmpty() const;

 private:
  // this header should be valid as a pure-cpp header
  // so we don't using Objective-C types
  // JSManagedObject* -> JscWeakRef.mm
  void* managedValue_ = nullptr;

  // if it's not a object, just keep the ordinary value
  JSValueRef nonObject_ = nullptr;
};

}  // namespace script::jsc_backend
