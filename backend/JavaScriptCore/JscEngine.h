
#pragma once

#include <JavaScriptCore/JavaScript.h>
#include <unordered_map>

#include "../../src/Engine.h"
#include "../../src/Native.h"
#include "../../src/utils/GlobalWeakBookkeeping.hpp"
#include "JscHelper.h"

namespace script::internal {

// forward declare
// defined in JscEngine.h
template <typename T, typename Args>
struct MakeLocalHelper;

}  // namespace script::internal

namespace script::jsc_backend {

class JscEngine : public ::script::ScriptEngine {
  struct ClassRegistryData {
    JSClassRef instanceClass{};
    Global<Object> constructor{};
    Global<Object> prototype{};
  };

  std::shared_ptr<utils::MessageQueue> messageQueue_;
  std::unordered_map<const void*, ClassRegistryData> classRegistry_;
  std::unordered_map<size_t, Global<Value>> keptReference_;
  internal::GlobalWeakBookkeeping globalWeakBookkeeping_;
  size_t keepId_ = 0;
  bool isDestroying_ = false;

  static JSClassRef globalClass_;
  static JSClassRef externalClass_;
  // >= ios10, >= macOS 10.12
  static bool hasByteBufferAPI_;

 protected:
  std::shared_ptr<std::recursive_mutex> virtualMachineLock_ =
      std::make_shared<std::recursive_mutex>();
  JSContextGroupRef virtualMachine_ = nullptr;
  JSGlobalContextRef context_ = nullptr;
  Global<Value> internalStorageSymbol_;
  Global<Value> constructorMarkSymbol_;
  Global<Function> getInternalStorageBySymbolFunction_;
  Global<Function> isByteBuffer_;

 private:
  static void checkException(JSValueRef exception);

 public:
  explicit JscEngine(std::shared_ptr<utils::MessageQueue> messageQueue = {});

  void destroy() noexcept override;

  bool isDestroying() const override { return isDestroying_; }

  Local<Object> getGlobal();

  Local<Value> get(const Local<String>& key) override;

  void set(const Local<String>& key, const Local<Value>& value) override;
  using ScriptEngine::set;

  Local<Value> eval(const Local<String>& script, const Local<String>& sourceFile) override;
  Local<Value> eval(const Local<String>& script) override;
  using ScriptEngine::eval;

  std::shared_ptr<utils::MessageQueue> messageQueue() override;

  void gc() override;

  void adjustAssociatedMemory(int64_t count) override;

  ScriptLanguage getLanguageType() override;

  std::string getEngineVersion() override;

 protected:
  ~JscEngine() override;

 private:
  Local<Value> eval(const Local<String>& script, const Local<Value>& sourceFile);

  template <typename T>
  bool registerNativeClassImpl(const ClassDefine<T>* classDefine);

  template <typename T>
  Local<Object> newNativeClassImpl(const ClassDefine<T>* classDefine, size_t size,
                                   const Local<Value>* args);

  template <typename T>
  bool isInstanceOfImpl(const Local<Value>& value, const ClassDefine<T>* classDefine);

  template <typename T>
  T* getNativeInstanceImpl(const Local<Value>& value, const ClassDefine<T>* classDefine);

 private:
  template <typename T>
  static inline typename RefTypeMap<T>::jscType toJsc(JSGlobalContextRef /*context*/,
                                                      const Local<T>& ref) {
    return ref.val_;
  }

  static inline typename RefTypeMap<String>::jscType toJsc(JSGlobalContextRef context,
                                                           const Local<String>& ref) {
    return ref.val_.getValue(context);
  }

  static typename RefTypeMap<Value>::jscType toJsc(JSGlobalContextRef context,
                                                   const Local<Value>& ref);

  static Arguments newArguments(JscEngine* engine, JSObjectRef thisObject,
                                const JSValueRef* arguments, size_t size);

  template <typename T, typename... Args>
  static T make(Args&&... args) {
    return T(std::forward<Args>(args)...);
  }

  Local<Function> newStaticFunction(const internal::StaticDefine::FunctionDefine& func);

  Local<Value> newStaticGetter(const internal::StaticDefine::PropertyDefine& prop);

  Local<Value> newStaticSetter(const internal::StaticDefine::PropertyDefine& prop);

  void registerStaticDefine(const internal::StaticDefine& staticDefine,
                            const Local<Object>& object);

  template <typename T>
  void defineInstance(const ClassDefine<T>* classDefine, Local<Value>& object,
                      ClassRegistryData& registry);

  template <typename T>
  JSObjectCallAsConstructorCallback createConstructor();

  template <typename T>
  Local<Object> defineInstancePrototype(const ClassDefine<T>* classDefine);

  template <typename T>
  void defineInstanceFunction(const ClassDefine<T>* classDefine, Local<Object>& prototypeObject);

  template <typename T, typename ConsumeLambda>
  void defineInstanceProperties(const ClassDefine<T>* classDefine, ConsumeLambda consumerLambda);

  size_t keepReference(const Local<Value>& ref);

  void removeKeptReference(size_t id);

  void initInternalSymbols();

  bool isConstructorMarkSymbol(JSValueRef value);

 private:
  template <typename T>
  friend class ::script::Local;

  template <typename T>
  friend class ::script::Global;

  template <typename T>
  friend class ::script::Weak;

  friend class ::script::Object;

  friend class ::script::Array;

  friend class ::script::Function;

  friend class ::script::ByteBuffer;

  friend class ::script::ScriptEngine;

  friend class ::script::Exception;

  friend class ::script::Arguments;

  friend class ::script::ScriptClass;

  friend class JscStringRefHolder;

  friend class JscWeakRef;

  friend JSGlobalContextRef currentEngineContextChecked();
  friend JSContextGroupRef currentEngineContextGroupChecked();

  friend class JscEngineScope;

  friend class JscExitEngineScope;

  friend class StringLocalRef;

  friend struct JscBookKeepFetcher;

  template <typename R, typename Fn>
  friend R toJscValues(JSGlobalContextRef context, size_t length, const Local<Value>* args, Fn fn);

  friend JSObjectRef valueToObj(JSGlobalContextRef context, JSValueRef value);

  friend struct ::script::jsc_interop;

  template <typename T, typename Args>
  friend struct ::script::internal::MakeLocalHelper;

  template <typename Ref>
  static auto& refVal(Ref* ref) {
    return ref->val_;
  }
};

}  // namespace script::jsc_backend
