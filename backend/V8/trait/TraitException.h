
#pragma once

#include "../../../src/foundation.h"
#include "../../../src/types.h"
#include "../V8Helper.h"

namespace script {

namespace v8_backend {

class ExceptionFields {
 public:
  mutable Global<Value> exception_{};
  mutable std::string message_{};
  mutable bool hasMessage_ = false;

  void fillMessage() const noexcept;
};

}  // namespace v8_backend

template <>
struct internal::ImplType<Exception> {
  using type = v8_backend::ExceptionFields;
};

}  // namespace script
