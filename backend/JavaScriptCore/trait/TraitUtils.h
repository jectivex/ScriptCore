
#pragma once
#include <JavaScriptCore/JavaScript.h>
#include "../../../src/foundation.h"
#include "../../../src/types.h"
#include "../JscHelper.h"
#include "TraitReference.h"

namespace script {
struct jsc_interop;

namespace jsc_backend {

struct StringHolderData {
  StringLocalRef::SharedStringRef jscStringRefHolder;
  mutable bool inited;
  mutable size_t length;
  mutable std::string stringContent;

  explicit StringHolderData(StringLocalRef::SharedStringRef r)
      : jscStringRefHolder(std::move(r)),
        inited(false),
        length(std::string::npos),
        stringContent() {}
};

}  // namespace jsc_backend

template <>
struct internal::ImplType<StringHolder> {
  using type = jsc_backend::StringHolderData;
};

template <>
struct internal::ImplType<internal::interop> {
  using type = jsc_interop;
};

}  // namespace script
