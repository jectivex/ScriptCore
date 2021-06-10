
#include <ScriptCore/ScriptCore.h>

namespace script {

StringHolder::StringHolder(const script::Local<script::String> &string) {}

StringHolder::~StringHolder() = default;

size_t StringHolder::length() const { return 0; }

const char *StringHolder::c_str() const { return ""; }

std::string_view StringHolder::stringView() const { return {}; }

std::string StringHolder::string() const { return {}; }

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
