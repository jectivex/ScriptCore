
#include "Scope.h"
#include "utils/ThreadLocal.h"
#include SCRIPTCORE_BACKEND(Includes.h)

namespace script {

SCRIPTCORE_THREAD_LOCAL(EngineScope*, current_);

EngineScope::EngineScope(ScriptEngine& engine) : EngineScope(&engine) {}

EngineScope::EngineScope(script::ScriptEngine* engine)
    : EngineScope(internal::scriptDynamicCast<EngineImpl*>(engine)) {}

EngineScope::EngineScope(EngineImpl& engine) : EngineScope(&engine) {}

EngineScope::EngineScope(EngineImpl* engine)
    : EngineScope(EngineScope::InternalEnterEngine{}, engine) {
  // public api require engine to be non-null
  if (!engine_) {
    throw std::logic_error("create EngineScope with a nullptr");
  }
}

static inline EngineScope*& currentScope() { return internal::getThreadLocal(current_); }

EngineScope::EngineScope(EngineScope::InternalEnterEngine, EngineImpl* engine, bool needEnter)
    : needEnter_(needEnter && engine != nullptr && engine != currentEngine()),
      engineScopeImpl_(),
      engine_(engine),
      prev_(currentScope()) {
  if (needEnter_) {
    if (engine->isDestroying()) {
      throw std::logic_error("enter EngineScope with a destroying ScriptEngine");
    }
    engineScopeImpl_.emplace(*engine);
  }

  currentScope() = this;
}

EngineScope::~EngineScope() {
  if (needEnter_) {
    engineScopeImpl_.reset();
  }
  currentScope() = prev_;
}

EngineScope* EngineScope::getCurrent() { return currentScope(); }

void EngineScope::ensureEngineScope(void* engine) {
  if (!engine) {
    throw std::logic_error("call engine APIs without a ::script::EngineScope");
  }
}

ScriptEngine* EngineScope::currentEngine() {
  auto& scope = currentScope();
  if (scope != nullptr) {
    return static_cast<ScriptEngine*>(scope->engine_);
  }
  return nullptr;
}

ScriptEngine& EngineScope::currentEngineChecked() {
  auto engine = EngineScope::currentEngine();
  ensureEngineScope(engine);
  return *engine;
}

ExitEngineScope::ExitEngineScope()
    : exitEngineScopeImpl_(EngineScope::currentEngineAs<EngineImpl>()),
      nullEngineScope_(EngineScope::InternalEnterEngine{}, nullptr) {}

ExitEngineScope::ExitEngineHolder::ExitEngineHolder(ExitEngineScope::EngineImpl* engine)
    : exitEngineHolder_() {
  if (engine) {
    exitEngineHolder_.emplace(*engine);
  }
}

ExitEngineScope::~ExitEngineScope() = default;

StackFrameScope::StackFrameScope()
    : stackFrameScopeImpl_(EngineScope::currentEngineCheckedAs<EngineImpl>()) {}

StackFrameScope::~StackFrameScope() = default;
}  // namespace script
