
#include "../../src/Exception.h"
#include "WasmEngine.h"
#include "WasmReference.hpp"

namespace script {

void wasm_backend::ExceptionFields::fillMessage() const noexcept {
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

Exception::Exception(std::string message) : std::exception(), exception_() {
  exception_.message_ = std::move(message);
  exception_.hasMessage_ = true;
}

Exception::Exception(const script::Local<script::String>& message)
    : std::exception(), exception_() {
  exception_.exception_ =
      wasm_backend::WasmEngine::make<Local<Value>>(wasm_backend::Stack::newError(message.val_));
#ifndef NDEBUG
  // better debug experience
  exception_.fillMessage();
#endif
}

Exception::Exception(const script::Local<script::Value>& exception)
    : std::exception(), exception_({}) {
  if (exception.isObject()) {
    exception_.exception_ = exception;
  } else {
    exception_.exception_ = wasm_backend::WasmEngine::make<Local<Value>>(
        wasm_backend::Stack::newError(exception.describe().val_));
  }
#ifndef NDEBUG
  // better debug experience
  exception_.fillMessage();
#endif
}

Local<Value> Exception::exception() const {
  if (exception_.exception_.isEmpty()) {
    exception_.exception_ = wasm_backend::WasmEngine::make<Local<Value>>(
        wasm_backend::Stack::newError(String::newString(exception_.message_).val_));
  }
  return exception_.exception_.getValue();
}

std::string Exception::message() const noexcept {
  exception_.fillMessage();
  return exception_.message_;
}

std::string Exception::stacktrace() const noexcept {
  try {
    auto exp = exception();
    if (exp.isObject()) {
      auto stack = exception().asObject().get("stack");
      if (stack.isString()) {
        return stack.asString().toString();
      }
    }
    return exp.describeUtf8();
  } catch (Exception&) {
  }
  return "[failed to obtain stacktrace]";
}

const char* Exception::what() const noexcept {
  exception_.fillMessage();
  return exception_.message_.c_str();
}

}  // namespace script
