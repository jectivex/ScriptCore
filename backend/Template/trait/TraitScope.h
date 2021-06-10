
#pragma once

#include "TraitEngine.h"

namespace script {

namespace template_backend {

class EngineScopeImpl {
 public:
  explicit EngineScopeImpl(TemplateEngine &) {
    // enter engine;
  }

  ~EngineScopeImpl() {
    // exit engine;
  }
};

class ExitEngineScopeImpl {
 public:
  explicit ExitEngineScopeImpl(TemplateEngine &) {
    // exit current entered engine
  }

  ~ExitEngineScopeImpl() {
    // reenter engine;
  }
};

class StackFrameScopeImpl {
 public:
  explicit StackFrameScopeImpl(TemplateEngine &) {
    // enter stack;
  }

  ~StackFrameScopeImpl() {
    // exit stack;
  }

  template <typename T>
  Local<T> returnValue(const Local<T> &localRef) {
    return localRef;
  }
};
}  // namespace template_backend

template <>
struct internal::ImplType<EngineScope> {
  using type = template_backend::EngineScopeImpl;
};

template <>
struct internal::ImplType<ExitEngineScope> {
  using type = template_backend::ExitEngineScopeImpl;
};

template <>
struct internal::ImplType<StackFrameScope> {
  using type = template_backend::StackFrameScopeImpl;
};

}  // namespace script
