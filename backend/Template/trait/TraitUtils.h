
#pragma once

namespace script {

template <>
struct internal::ImplType<StringHolder> {
  using type = int;
};

template <>
struct internal::ImplType<internal::interop> {
  using type = int;
};

}  // namespace script
