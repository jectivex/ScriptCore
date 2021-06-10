
#include "V8Scope.h"
#include "V8Engine.h"
#include "V8Helper.h"
#include "V8Reference.hpp"

namespace script::v8_backend {

V8EngineScope::V8EngineScope(V8Engine& engine)
    : locker_(engine.isolate_),
      isolateScope_(engine.isolate_),
      handleScope_(engine.isolate_),
      contextScope_(engine.context_.Get(engine.isolate_)) {}

V8ExitEngineScope::V8ExitEngineScope(V8Engine& engine) : unlocker_(engine.isolate_) {}

V8HandleScope::V8HandleScope(V8Engine& engine) : handleScope_(engine.isolate_) {}

}  // namespace script::v8_backend
