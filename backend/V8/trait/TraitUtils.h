
#pragma once
#include "../../../src/foundation.h"
#include "../../../src/types.h"
#include "../V8Helper.h"

namespace script {

template <>
struct internal::ImplType<StringHolder> {
  using type = v8::String::Utf8Value;
};

template <>
struct internal::ImplType<internal::interop> {
  using type = v8_interop;
};

}  // namespace script
