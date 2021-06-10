
#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include "../../src/Native.hpp"
#include "WasmEngine.h"

namespace script::wasm_backend {

template <typename T>
void WasmEngine::registerNativeClassImpl(const ClassDefine<T>* classDefine) {
  if (classDefineRegistry_.find(classDefine) != classDefineRegistry_.end()) {
    throw Exception("classDefine [" + classDefine->className + "] already registered");
  }

  StackFrameScope scope;

  auto hasInstance = classDefine->instanceDefine.constructor != nullptr;
  auto obj = hasInstance ? newConstructor(classDefine) : Object::newObject();

  defineStatic(obj, classDefine->staticDefine);

  if (hasInstance) {
    defineInstance(classDefine, obj);
  }

  auto ns = getNamespaceForRegister(classDefine->nameSpace);
  ns.set(classDefine->className, obj);

  classDefineRegistry_.emplace(classDefine, obj);
}

template <typename T>
Local<Object> WasmEngine::newNativeClassImpl(const ClassDefine<T>* classDefine, size_t size,
                                             const Local<Value>* args) {
  auto it = classDefineRegistry_.find(classDefine);
  if (it == classDefineRegistry_.end()) {
    throw Exception("classDefine [" + classDefine->className + "] is not registered");
  }

  StackFrameScope scope;
  auto ctor = it->second.get();
  auto ret = Object::newObjectImpl(ctor, size, args);
  return scope.returnValue(ret);
}

template <typename T>
bool WasmEngine::isInstanceOfImpl(const Local<Value>& value, const ClassDefine<T>* classDefine) {
  return NativeHelper::getInternalStateClassDefine(value.val_) == classDefine;
}

template <typename T>
T* WasmEngine::getNativeInstanceImpl(const Local<Value>& value, const ClassDefine<T>* classDefine) {
  if (isInstanceOfImpl(value, classDefine)) {
    return static_cast<T*>(NativeHelper::getInternalStateInstance(value.val_));
  }
  return nullptr;
}

template <typename T>
Local<Object> WasmEngine::newConstructor(const ClassDefine<T>* classDefine) {
  auto ctor = Stack::newFunction(
      [](const Arguments& args, void* data, void*) -> Local<Value> {
        auto classDefine = static_cast<ClassDefine<T>*>(data);

        Tracer trace(args.engine(), classDefine->className);
        T* instance;
        if (args.size() == 2 && NativeHelper::isCppNewMark(args[0].val_)) {
          // WASM is 32-bit, we could use int32_t to store a pointer.
          // we have static assert in WasmNative.hpp
          instance = reinterpret_cast<T*>(args[1].asNumber().toInt32());
        } else {
          instance = classDefine->instanceDefine.constructor(args);
          if (instance == nullptr) {
            throw Exception("can't create class " + classDefine->className);
          }
        }
        NativeHelper::setInternalState(args.thiz().val_, classDefine, instance);
        return {};
      },
      classDefine, nullptr, true);
  return Local<Object>(ctor);
}

template <typename T>
void WasmEngine::defineInstance(const ClassDefine<T>* classDefine, const Local<Object>& obj) {
  auto&& instanceDefine = classDefine->instanceDefine;
  auto prototype = Object::newObject();

  for (auto&& func : instanceDefine.functions) {
    StackFrameScope stack;
    auto fi = Stack::newFunction(
        [](const Arguments& args, void* data0, void* data1) -> Local<Value> {
          auto classDefine = static_cast<const ClassDefine<T>*>(data0);
          auto& func = *static_cast<typename internal::InstanceDefine<T>::FunctionDefine*>(data1);

          auto ins = verifyAndGetInstance(classDefine, args.thiz().val_);

          Tracer trace(args.engine(), func.traceName);
          return func.callback(reinterpret_cast<T*>(ins), args);
        },
        classDefine, &func);

    prototype.set(func.name, Local<Value>(fi));
  }

  for (auto&& prop : instanceDefine.properties) {
    StackFrameScope stackFrame;

    int getter = -1;
    int setter = -1;
    auto name = String::newString(prop.name);

    if (prop.getter) {
      getter = Stack::newFunction(
          [](const Arguments& args, void* data0, void* data1) -> Local<Value> {
            auto& prop = *static_cast<typename internal::InstanceDefine<T>::PropertyDefine*>(data0);
            auto classDefine = static_cast<const ClassDefine<T>*>(data1);

            auto ins = verifyAndGetInstance(classDefine, args.thiz().val_);

            Tracer trace(args.engine(), prop.traceName);
            return prop.getter(reinterpret_cast<T*>(ins));
          },
          &prop, classDefine);
    }

    if (prop.setter) {
      setter = Stack::newFunction(
          [](const Arguments& args, void* data0, void* data1) -> Local<Value> {
            auto& prop = *static_cast<typename internal::InstanceDefine<T>::PropertyDefine*>(data0);
            auto classDefine = static_cast<const ClassDefine<T>*>(data1);

            auto ins = verifyAndGetInstance(classDefine, args.thiz().val_);
            Tracer trace(args.engine(), prop.traceName);
            prop.setter(reinterpret_cast<T*>(ins), args[0]);
            return {};
          },
          &prop, classDefine);
    }
    NativeHelper::defineProperty(prototype.val_, name.val_, getter, setter);
  }

  // set the `prototype` property of constructor
  obj.set("prototype", prototype);
}

}  // namespace script::wasm_backend
