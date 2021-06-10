
#include <ScriptCore/ScriptCore.h>

namespace script {

namespace lua_backend {

void ExceptionFields::fillMessage() const noexcept {
  if (!hasMessage_) {
    hasMessage_ = true;

    StackFrameScope stack;
    try {
      luaEnsureStack(currentLua(), 2);
      auto error = exception_.get();
      if (error.isObject()) {
        auto msg = error.asObject().get("message");
        if (!msg.isNull()) {
          message_ = msg.describeUtf8();
          return;
        }
      }
      if (!error.isNull()) {
        message_ = error.describeUtf8();
      } else {
        message_ = "unknown error";
      }
    } catch (const Exception&) {
      message_ = "unknown error";
    }
  }
}

void ExceptionFields::fillStacktrace() const noexcept {
  if (!hasStacktrace_) {
    hasStacktrace_ = true;

    StackFrameScope stackScope;
    try {
      luaEnsureStack(currentLua(), 2);

      auto error = exception_.get();

      if (error.isObject()) {
        auto stack = error.asObject().get("stack");
        if (!stack.isNull()) {
          stacktrace_ = stack.describeUtf8();
        }
      }
    } catch (const Exception&) {
      stacktrace_ = "[failed to get stacktrace]";
    }
  }
}

}  // namespace lua_backend

Exception::Exception(std::string message) : std::exception(), exception_() {
  exception_.message_ = std::move(message);
  exception_.hasMessage_ = true;
}

Exception::Exception(const script::Local<script::String>& message)
    : std::exception(), exception_() {
  exception_.exception_ = message;

  exception_.hasStacktrace_ = true;
  // get stacktrace

#ifndef NDEBUG
  exception_.fillMessage();
#endif
}

Exception::Exception(const script::Local<script::Value>& exception)
    : std::exception(), exception_({}) {
  exception_.exception_ = exception;
}

Local<Value> Exception::exception() const {
  if (exception_.exception_.isEmpty()) {
    exception_.exception_ = String::newString(exception_.message_);
  }
  return exception_.exception_.getValue();
}

std::string Exception::message() const noexcept {
  exception_.fillMessage();
  return exception_.message_;
}

std::string Exception::stacktrace() const noexcept {
  exception_.fillStacktrace();
  return exception_.stacktrace_;
}

const char* Exception::what() const noexcept {
  exception_.fillMessage();
  return exception_.message_.c_str();
}

}  // namespace script
