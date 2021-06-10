
#include "QjsEngine.hpp"
#include "trait/TraitScope.h"

namespace script::qjs_backend {

EngineScopeImpl::EngineScopeImpl(QjsEngine &current)
    : previous_(EngineScope::currentEngineAs<QjsEngine>()), current_(&current) {
  if (previous_) {
    previous_->runtimeLock_.unlock();
  }
  current_->runtimeLock_.lock();
}

EngineScopeImpl::~EngineScopeImpl() {
  current_->runtimeLock_.unlock();
  if (previous_) {
    previous_->runtimeLock_.lock();
  }
}

ExitEngineScopeImpl::ExitEngineScopeImpl(QjsEngine &current) : current_(&current) {
  current_->runtimeLock_.unlock();
}

ExitEngineScopeImpl::~ExitEngineScopeImpl() { current_->runtimeLock_.lock(); }

}  // namespace script::qjs_backend
