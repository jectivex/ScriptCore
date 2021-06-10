
#include "JscScope.h"
#include "../../src/Scope.h"
#include "JscEngine.h"

namespace script::jsc_backend {

JscEngineScope::JscEngineScope(JscEngine& engine)
    : unlockPrevious_(&engine), lockGuard_(*engine.virtualMachineLock_) {}

void JscEngineScope::UnlockPrevious_Ctor(JscEngine* currentEngine,
                                         JscEngineScope::UnlockPrevious& u) {
  // we are building new frame, so "current" is actually the previous frame.
  auto previous = ::script::EngineScope::currentEngineAs<JscEngine>();
  if (previous && previous != currentEngine) {
    u.previousEngine_ = previous;
    previous->virtualMachineLock_->unlock();
  }
}

void JscEngineScope::UnlockPrevious_Dtor(JscEngineScope::UnlockPrevious& u) {
  if (u.previousEngine_) {
    u.previousEngine_->virtualMachineLock_->lock();
  }
}

JscEngineScope::~JscEngineScope() = default;

JscExitEngineScope::JscExitEngineScope(JscEngine& engine)
    : virtualMachineLock_(*engine.virtualMachineLock_) {
  virtualMachineLock_.unlock();
}

JscExitEngineScope::~JscExitEngineScope() { virtualMachineLock_.lock(); }

}  // namespace script::jsc_backend
