
#pragma once

#include "../../../src/foundation.h"
#include "../../../src/types.h"
#include "../V8Helper.h"
#include "../V8Scope.h"

namespace script {

template <>
struct internal::ImplType<EngineScope> {
  using type = v8_backend::V8EngineScope;
};

template <>
struct internal::ImplType<ExitEngineScope> {
  using type = v8_backend::V8ExitEngineScope;
};

template <>
struct internal::ImplType<StackFrameScope> {
  using type = v8_backend::V8HandleScope;
};

}  // namespace script
