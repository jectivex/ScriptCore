
#include <cstdlib>
#include "../../src/Utils.h"
#include "WasmEngine.h"

namespace script {

static void fillString(const wasm_backend::WasmStringHolder &h) {
  if (!h.cString_) {
    h.cString_ = wasm_backend::Stack::toCString(h.index_);
    if (h.cString_) {
      h.length_ = std::strlen(h.cString_);
    } else {
      h.cString_ = "[script::StringHolder toString failed]";
      h.length_ = 0;
    }
  }
}

StringHolder::StringHolder(const script::Local<script::String> &string)
    : internalHolder_{nullptr, 0, string.val_} {}

StringHolder::~StringHolder() {
  if (internalHolder_.cString_) {
    std::free(const_cast<char *>(internalHolder_.cString_));
  }
}

size_t StringHolder::length() const {
  fillString(internalHolder_);
  return internalHolder_.length_;
}

const char *StringHolder::c_str() const {
  fillString(internalHolder_);
  return internalHolder_.cString_;
}

std::string_view StringHolder::stringView() const {
  fillString(internalHolder_);
  return std::string_view(internalHolder_.cString_, internalHolder_.length_);
}

std::string StringHolder::string() const {
  fillString(internalHolder_);
  return internalHolder_.cString_;
}

#if defined(__cpp_char8_t)
// NOLINTNEXTLINE(clang-analyzer-cplusplus.InnerPointer)
std::u8string StringHolder::u8string() const { return std::u8string(c_u8str(), length()); }

std::u8string_view StringHolder::u8stringView() const {
  // NOLINTNEXTLINE(clang-analyzer-cplusplus.InnerPointer)
  return std::u8string_view(c_u8str(), length());
}

const char8_t *StringHolder::c_u8str() const { return reinterpret_cast<const char8_t *>(c_str()); }
#endif

}  // namespace script
