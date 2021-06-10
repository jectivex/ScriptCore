
#pragma once

#include "../../../src/foundation.h"
#include "../../src/types.h"
#include "../JscScope.h"

namespace script {

template <>
struct internal::ImplType<EngineScope> {
  using type = jsc_backend::JscEngineScope;
};

template <>
struct internal::ImplType<ExitEngineScope> {
  using type = jsc_backend::JscExitEngineScope;
};

template <>
struct internal::ImplType<StackFrameScope> {
  using type = jsc_backend::JscStackFrameScope;
};

}  // namespace script
