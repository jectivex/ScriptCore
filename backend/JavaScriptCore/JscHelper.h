
#pragma once

#include <JavaScriptCore/JavaScript.h>
#include "../../src/types.h"

namespace script::jsc_backend {

// forward declare to solve recursive include issue
class JscEngine;

JscEngine* currentEngine();

JscEngine& currentEngineChecked();

JSGlobalContextRef currentEngineContextChecked();

JSContextGroupRef currentEngineContextGroupChecked();

template <typename R, typename Fn>
R toJscValues(JSGlobalContextRef context, size_t length, const Local<Value>* args, Fn fn);

}  // namespace script::jsc_backend

namespace script {
struct jsc_interop;
}
