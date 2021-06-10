
#pragma once

#include "../../src/types.h"

SCRIPTCORE_BEGIN_INCLUDE_LIBRARY
#include <v8.h>
SCRIPTCORE_END_INCLUDE_LIBRARY

namespace script::v8_backend {

class V8Engine;

// forward declare to solve recursive include issue
// implemented in V8Engine.h
V8Engine* currentEngine();

V8Engine& currentEngineChecked();

v8::Isolate* currentEngineIsolateChecked();

v8::Local<v8::Context> currentEngineContextChecked();

std::tuple<v8::Isolate*, v8::Local<v8::Context>> currentEngineIsolateAndContextChecked();

void checkException(v8::TryCatch& tryCatch);

void rethrowException(const Exception& exception);

}  // namespace script::v8_backend

namespace script {
struct v8_interop;
}
