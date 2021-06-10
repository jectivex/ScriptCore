
#include <ScriptCore/ScriptCore.h>

namespace script {

StringHolder::StringHolder(const script::Local<script::String> &string) : internalHolder_({}) {
  internalHolder_.context_ = qjs_backend::currentContext();
  auto str = JS_ToCStringLen(internalHolder_.context_, &internalHolder_.length_,
                             qjs_interop::peekLocal(string));
  if (str == nullptr) {
    throw Exception("failed to get string from Local<String>");
  }
  internalHolder_.string_ = str;
}

StringHolder::~StringHolder() { JS_FreeCString(internalHolder_.context_, internalHolder_.string_); }

size_t StringHolder::length() const { return internalHolder_.length_; }

const char *StringHolder::c_str() const { return internalHolder_.string_; }

std::string_view StringHolder::stringView() const {
  return std::string_view(internalHolder_.string_, internalHolder_.length_);
}

std::string StringHolder::string() const { return std::string(stringView()); }

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
