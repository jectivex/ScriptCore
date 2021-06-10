
#include "V8Helper.hpp"
#include "V8Engine.h"

namespace script::v8_backend {

// helper
V8Engine* currentEngine() { return ::script::EngineScope::currentEngineAs<V8Engine>(); }

V8Engine& currentEngineChecked() {
  return ::script::EngineScope::currentEngineCheckedAs<V8Engine>();
}

v8::Isolate* currentEngineIsolateChecked() { return currentEngineChecked().isolate_; }

v8::Local<v8::Context> currentEngineContextChecked() {
  auto isolate = currentEngineIsolateChecked();
  return currentEngineChecked().context_.Get(isolate);
}

std::tuple<v8::Isolate*, v8::Local<v8::Context>> currentEngineIsolateAndContextChecked() {
  auto& engine = currentEngineChecked();
  return std::make_tuple(engine.isolate_, engine.context_.Get(engine.isolate_));
}

void checkException(v8::TryCatch& tryCatch) {
  if (tryCatch.HasCaught()) {
    throw Exception(v8_backend::V8Engine::make<Local<Value>>(tryCatch.Exception()));
  }
}

void rethrowException(const Exception& exception) {
  auto isolate = v8_backend::currentEngineIsolateChecked();
  isolate->ThrowException(v8_backend::V8Engine::toV8(isolate, exception.exception()));
}

}  // namespace script::v8_backend
