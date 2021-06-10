
#include "Reference.h"
#include "Scope.h"
#include "Utils.h"

namespace script {

std::string Local<Value>::describeUtf8() const {
  if (isNull()) return "null";
  return describe().toString();
}

StringHolder Local<String>::toStringHolder() const { return StringHolder(*this); }

std::string Local<String>::toString() const { return toStringHolder().string(); }

#ifdef __cpp_char8_t
std::u8string Local<String>::toU8string() const { return toStringHolder().u8string(); }
#endif

std::vector<std::string> Local<Object>::getKeyNames() const {
  std::vector<std::string> ret;
  script::StackFrameScope stack;
  for (auto&& k : getKeys()) {
    ret.push_back(k.toString());
  }
  return ret;
}

}  // namespace script
