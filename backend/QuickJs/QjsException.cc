
#include <ScriptCore/ScriptCore.h>

namespace script {

namespace qjs_backend {

void ExceptionFields::fillMessage() const {
  if (!hasMessage_) {
    hasMessage_ = true;
    try {
      auto exp = exception_.get().asObject();
      message_ = exp.get("message").asString().toString();
    } catch (Exception&) {
      message_ = "[another exception in message()]";
    }
  }
}

}  // namespace qjs_backend

Exception::Exception(std::string msg) : std::exception(), exception_() {
  exception_.message_ = std::move(msg);
  exception_.hasMessage_ = true;
}

Exception::Exception(const script::Local<script::String>& message)
    : std::exception(), exception_() {
  exception_.message_ = message.toString();
  exception_.hasMessage_ = true;
}

Exception::Exception(const script::Local<script::Value>& exception)
    : std::exception(), exception_() {
  exception_.exception_ = exception;
}

Local<Value> Exception::exception() const {
  if (exception_.exception_.isEmpty()) {
    auto context = qjs_backend::currentContext();
    JS_ThrowReferenceError(context, "%s", message().c_str());
    exception_.exception_ = qjs_interop::makeLocal<Value>(JS_GetException(context));
  }
  return exception_.exception_.getValue();
}

std::string Exception::message() const noexcept {
  exception_.fillMessage();
  return exception_.message_;
}

std::string Exception::stacktrace() const noexcept {
  try {
    return exception().asObject().get("stack").asString().toString();
  } catch (Exception&) {
    return "[failed to obtain stacktrace]";
  }
}
const char* Exception::what() const noexcept {
  exception_.fillMessage();
  return exception_.message_.c_str();
}

}  // namespace script
