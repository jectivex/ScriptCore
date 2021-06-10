
#include "Engine.h"
#include "Native.hpp"

namespace script {

void ScriptEngine::setData(std::shared_ptr<void> arbitraryData) {
  userData_ = std::move(arbitraryData);
}

void ScriptEngine::registerNativeClass(const NativeRegister& nativeRegister) {
  nativeRegister.registerNativeClass(this);
}

void ScriptEngine::destroyUserData() { userData_.reset(); }

}  // namespace script
