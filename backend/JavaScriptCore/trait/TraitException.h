
#pragma once

#include "../../../src/Reference.h"
#include "../../../src/foundation.h"
#include "../../../src/types.h"

namespace script {

namespace jsc_backend {

class ExceptionFields {
 public:
  mutable Global<Value> exception_{};
  mutable std::string message_{};
  mutable bool hasMessage_ = false;

  void fillMessage() const;
};

}  // namespace jsc_backend

template <>
struct internal::ImplType<Exception> {
  using type = jsc_backend::ExceptionFields;
};

}  // namespace script
