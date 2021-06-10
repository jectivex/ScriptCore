
#pragma once

namespace script {

namespace template_backend {

class ExceptionFields {
 public:
  mutable std::string message_{};
};

}  // namespace template_backend

template <>
struct internal::ImplType<Exception> {
  using type = template_backend::ExceptionFields;
};

}  // namespace script
