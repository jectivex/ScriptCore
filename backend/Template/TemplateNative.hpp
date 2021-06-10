
#pragma once

#include "../../src/Native.h"
#include "TemplateEngine.h"

namespace script {

template <typename T>
ScriptClass::ScriptClass(const ScriptClass::ConstructFromCpp<T>) : internalState_() {}

template <typename T>
T* Arguments::engineAs() const {
  return nullptr;
}

}  // namespace script
