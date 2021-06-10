
#include "JscHelper.h"
#include "../../src/Scope.h"
#include "JscEngine.h"

namespace script::jsc_backend {

JscEngine* currentEngine() { return EngineScope::currentEngineAs<JscEngine>(); }

JscEngine& currentEngineChecked() { return EngineScope::currentEngineCheckedAs<JscEngine>(); }

JSGlobalContextRef currentEngineContextChecked() { return currentEngineChecked().context_; }

JSContextGroupRef currentEngineContextGroupChecked() {
  return currentEngineChecked().virtualMachine_;
}

}  // namespace script::jsc_backend
