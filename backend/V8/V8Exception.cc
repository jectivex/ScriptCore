
#include <sstream>
#include <utility>
#include "../../src/Exception.h"
#include "../../src/Native.hpp"
#include "../../src/Scope.h"
#include "../../src/Utils.h"
#include "V8Engine.h"
#include "V8Helper.h"
#include "V8Reference.hpp"

namespace script {

void v8_backend::ExceptionFields::fillMessage() const noexcept {
  if (!hasMessage_) {
    hasMessage_ = true;
    try {
      auto isolate = v8_backend::currentEngineIsolateChecked();
      auto exp = v8_backend::V8Engine::toV8(isolate, exception_.get().asObject());
      auto msg = v8::Exception::CreateMessage(isolate, exp);
      if (!msg.IsEmpty()) {
        message_ = v8_backend::V8Engine::make<Local<String>>(msg->Get()).describeUtf8();
      }
    } catch (Exception&) {
      message_ = "unknown";
    }
  }
}

Exception::Exception(std::string message) : std::exception(), exception_({}) {
  exception_.message_ = std::move(message);
  exception_.hasMessage_ = true;
}

Exception::Exception(const script::Local<script::String>& message)
    : std::exception(), exception_({}) {
  exception_.exception_ =
      Global<Value>(v8_backend::V8Engine::make<Local<Value>>(v8::Exception::Error(
          v8_backend::V8Engine::toV8(v8_backend::currentEngineIsolateChecked(), message))));
}

Exception::Exception(const script::Local<script::Value>& exception)
    : std::exception(), exception_() {
  exception_.exception_ = Global<Value>(exception);
}

Local<Value> Exception::exception() const {
  if (exception_.exception_.isEmpty()) {
    auto message = String::newString(exception_.message_);

    exception_.exception_ =
        Global<Value>(v8_backend::V8Engine::make<Local<Value>>(v8::Exception::Error(
            v8_backend::V8Engine::toV8(v8_backend::currentEngineIsolateChecked(), message))));
  }
  return exception_.exception_.getValue();
}

std::string Exception::message() const noexcept {
  exception_.fillMessage();
  return exception_.message_;
}

std::string Exception::stacktrace() const noexcept {
  auto isolate = v8_backend::currentEngineIsolateChecked();

  v8::TryCatch tryCatch(isolate);
  auto exp = v8_backend::V8Engine::toV8(isolate, exception());
  auto msg = v8::Exception::CreateMessage(isolate, exp);
  if (!msg.IsEmpty()) {
    StackFrameScope scope;
    auto stack = msg->GetStackTrace();
    if (stack.IsEmpty()) {
      return {};
    }
    std::ostringstream stackStr;
    for (int i = 0, frameCount = stack->GetFrameCount(); i < frameCount; ++i) {
      v8::Local<v8::StackFrame> frame = stack->GetFrame(isolate, i);
      v8::Local<v8::String> script = frame->GetScriptName();

      v8::String::Utf8Value scriptName_(isolate, script);
      std::string_view scriptName;
      if (*scriptName_) {
        scriptName = *scriptName_;
      }

      v8::Local<v8::String> func = frame->GetFunctionName();
      v8::String::Utf8Value funcName_(isolate, func);
      std::string_view funcName;
      if (*funcName_) {
        funcName = *funcName_;
      }

      stackStr << "    at ";
      if (!funcName.empty()) {
        stackStr << funcName << " (";
      }

      stackStr << (scriptName.empty() ? "<anonymous>" : scriptName) << ":" << frame->GetLineNumber()
               << ":" << frame->GetColumn();
      if (!funcName.empty()) {
        stackStr << ") ";
      }

      if (i < (frameCount - 1)) {
        stackStr << "\n";
      }
    }
    return stackStr.str();
  }
  return {};
}

const char* Exception::what() const noexcept {
  exception_.fillMessage();
  return exception_.message_.c_str();
}

}  // namespace script
