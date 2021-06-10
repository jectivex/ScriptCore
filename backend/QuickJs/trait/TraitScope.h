
#pragma once

#include "TraitEngine.h"

namespace script {

namespace qjs_backend {

class EngineScopeImpl {
  QjsEngine *previous_;
  QjsEngine *current_;

 public:
  explicit EngineScopeImpl(QjsEngine &);

  ~EngineScopeImpl();
};

class ExitEngineScopeImpl {
  QjsEngine *current_;

 public:
  explicit ExitEngineScopeImpl(QjsEngine &);

  ~ExitEngineScopeImpl();
};

class StackFrameScopeImpl {
 public:
  // enter stack;
  explicit StackFrameScopeImpl(QjsEngine &){};

  // exit stack;
  ~StackFrameScopeImpl() = default;

  template <typename T>
  Local<T> returnValue(const Local<T> &localRef) {
    return localRef;
  }
};
}  // namespace qjs_backend

template <>
struct internal::ImplType<EngineScope> {
  using type = qjs_backend::EngineScopeImpl;
};

template <>
struct internal::ImplType<ExitEngineScope> {
  using type = qjs_backend::ExitEngineScopeImpl;
};

template <>
struct internal::ImplType<StackFrameScope> {
  using type = qjs_backend::StackFrameScopeImpl;
};

}  // namespace script
