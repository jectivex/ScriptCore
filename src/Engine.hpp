
#pragma once

#include <string>

#include "Engine.h"
#include "Exception.h"
#include "Reference.h"
#include SCRIPTCORE_BACKEND(Engine.h)
#include SCRIPTCORE_BACKEND(Includes.h)

namespace script {

template <typename T>
void ScriptEngine::registerNativeClass(const ClassDefine<T>& classDefine) {
  if ((std::is_same_v<void, T> &&
       staticClassDefineRegistry_.find(&classDefine) != staticClassDefineRegistry_.end()) ||
      classDefineRegistry_.find(internal::typeIndexOf<T>()) != classDefineRegistry_.end()) {
    throw Exception(std::string("already registered for " + classDefine.getClassName()));
  }
  using RealEngine = typename internal::ImplType<ScriptEngine>::type;

  internal::scriptDynamicCast<RealEngine*>(this)->registerNativeClassImpl<T>(&classDefine);

  if (std::is_same_v<void, T>) {
    staticClassDefineRegistry_.emplace(&classDefine);
  } else {
    classDefineRegistry_.emplace(internal::typeIndexOf<T>(), &classDefine);
  }
}

template <typename T>
const ClassDefine<T>& ScriptEngine::getClassDefine() const {
  static_assert(!std::is_same_v<void, T>);
  auto it = classDefineRegistry_.find(internal::typeIndexOf<T>());
  if (it == classDefineRegistry_.end()) {
    throw Exception(std::string("ClassDefine is not registered"));
  }
  return *static_cast<const ClassDefine<T>*>(it->second);
}

template <typename T>
Local<Object> ScriptEngine::newNativeClassImpl(const ClassDefine<T>& classDefine, size_t size,
                                               const Local<Value>* args) {
  static_assert(!std::is_same_v<void, T>);
  using RealEngine = typename internal::ImplType<ScriptEngine>::type;
  return internal::scriptDynamicCast<RealEngine*>(this)->newNativeClassImpl<T>(&classDefine, size,
                                                                               args);
}

template <typename T>
bool ScriptEngine::isInstanceOfImpl(const Local<Value>& value, const ClassDefine<T>& classDefine) {
  static_assert(!std::is_same_v<void, T>);
  using RealEngine = typename internal::ImplType<ScriptEngine>::type;
  return internal::scriptDynamicCast<RealEngine*>(this)->isInstanceOfImpl(value, &classDefine);
}

template <typename T>
T* ScriptEngine::getNativeInstanceImpl(const Local<Value>& value,
                                       const ClassDefine<T>& classDefine) {
  static_assert(!std::is_same_v<void, T>);
  using RealEngine = typename internal::ImplType<ScriptEngine>::type;
  return internal::scriptDynamicCast<RealEngine*>(this)->getNativeInstanceImpl(value, &classDefine);
}

template <typename T>
inline std::shared_ptr<T> ScriptEngine::getData() {
  return std::static_pointer_cast<T>(userData_);
}

}  // namespace script
