
#include "../../src/Utils.h"
#include "V8Engine.h"
#include "V8Helper.h"
#include "V8Reference.hpp"

namespace script {

StringHolder::StringHolder(const script::Local<script::String>& string)
    : internalHolder_(
          v8_backend::currentEngineIsolateChecked(),
          v8_backend::V8Engine::toV8(v8_backend::currentEngineIsolateChecked(), string)) {}

StringHolder::~StringHolder() = default;

size_t StringHolder::length() const { return static_cast<size_t>(internalHolder_.length()); }

const char* StringHolder::c_str() const { return *internalHolder_; }

std::string_view StringHolder::stringView() const { return std::string_view(c_str(), length()); }

std::string StringHolder::string() const { return std::string(c_str(), length()); }

#if defined(__cpp_char8_t)
std::u8string StringHolder::u8string() const { return std::u8string(c_u8str(), length()); }

std::u8string_view StringHolder::u8stringView() const {
  return std::u8string_view(c_u8str(), length());
}

const char8_t* StringHolder::c_u8str() const { return reinterpret_cast<const char8_t*>(c_str()); }
#endif
}  // namespace script
