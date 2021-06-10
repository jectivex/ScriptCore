
#pragma once

#include <mutex>
#include "../../src/types.h"

namespace script::jsc_backend {

class JscEngine;

class JscEngineScope {
 private:
  struct UnlockPrevious {
    JscEngine* previousEngine_;

    explicit UnlockPrevious(JscEngine* currentEngine) : previousEngine_() {
      UnlockPrevious_Ctor(currentEngine, *this);
    }

    ~UnlockPrevious() { UnlockPrevious_Dtor(*this); }
  };

  UnlockPrevious unlockPrevious_;
  std::lock_guard<std::recursive_mutex> lockGuard_;

 public:
  explicit JscEngineScope(JscEngine&);

  ~JscEngineScope();

 private:
  static void UnlockPrevious_Ctor(JscEngine* engine, UnlockPrevious&);

  static void UnlockPrevious_Dtor(UnlockPrevious&);
};

class JscExitEngineScope {
 private:
  std::recursive_mutex& virtualMachineLock_;

 public:
  explicit JscExitEngineScope(JscEngine&);

  ~JscExitEngineScope();
};

class JscStackFrameScope {
 public:
  explicit JscStackFrameScope(JscEngine&) {}

  template <typename T>
  Local<T> returnValue(const Local<T>& localRef) {
    return localRef;
  }
};

}  // namespace script::jsc_backend
