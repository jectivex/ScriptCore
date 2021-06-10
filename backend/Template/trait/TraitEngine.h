
#pragma once

#include <string>
#include "../../src/types.h"

#define TEMPLATE_NOT_IMPLEMENTED() throw Exception(std::string(__func__) + " not implemented");

namespace script {

namespace template_backend {
class TemplateEngine;
}

template <>
struct internal::ImplType<ScriptEngine> {
  using type = template_backend::TemplateEngine;
};

}  // namespace script
