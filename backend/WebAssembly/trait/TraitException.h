
#pragma once

#include <string>
#include "../../../src/types.h"

namespace script {

namespace wasm_backend {

class ExceptionFields {
 public:
  mutable Global<Value> exception_{};
  mutable std::string message_{};
  mutable bool hasMessage_ = false;

  void fillMessage() const noexcept;
};

}  // namespace wasm_backend

template <>
struct internal::ImplType<Exception> {
  using type = wasm_backend::ExceptionFields;
};

}  // namespace script
