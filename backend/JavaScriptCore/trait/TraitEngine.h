
#pragma once

#include "../../../src/foundation.h"
#include "../../../src/types.h"

namespace script {

namespace jsc_backend {
class JscEngine;
}

template <>
struct internal::ImplType<ScriptEngine> {
  using type = ::script::jsc_backend::JscEngine;
};

}  // namespace script
