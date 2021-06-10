
#pragma once

#include "../../../src/foundation.h"
#include "../../../src/types.h"

namespace script {
namespace v8_backend {
class V8Engine;
}

template <>
struct internal::ImplType<ScriptEngine> {
  using type = v8_backend::V8Engine;
};

}  // namespace script
