
#pragma once

#include <string>
#include "../../src/types.h"

namespace script {

namespace qjs_backend {
class QjsEngine;
}

template <>
struct internal::ImplType<ScriptEngine> {
  using type = qjs_backend::QjsEngine;
};

}  // namespace script
