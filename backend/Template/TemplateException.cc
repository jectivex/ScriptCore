
#include <ScriptCore/ScriptCore.h>

namespace script {

Exception::Exception(std::string msg) : std::exception(), exception_({}) {
  exception_.message_ = std::move(msg);
}

Exception::Exception(const script::Local<script::String>& message)
    : std::exception(), exception_() {}

Exception::Exception(const script::Local<script::Value>& exception)
    : std::exception(), exception_({}) {}

Local<Value> Exception::exception() const { return {}; }

std::string Exception::message() const noexcept { return exception_.message_; }

std::string Exception::stacktrace() const noexcept { return "[no stacktrace]"; }

const char* Exception::what() const noexcept { return exception_.message_.c_str(); }

}  // namespace script
