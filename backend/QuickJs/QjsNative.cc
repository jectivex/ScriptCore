
#include <ScriptCore/ScriptCore.h>

namespace script {

Arguments::Arguments(InternalCallbackInfoType callbackInfo) : callbackInfo_(callbackInfo) {}

Arguments::~Arguments() = default;

Local<Object> Arguments::thiz() const {
  return qjs_interop::makeLocal<Value>(
             qjs_backend::dupValue(callbackInfo_.thiz_, callbackInfo_.engine_->context_))
      .asObject();
}

bool Arguments::hasThiz() const { return JS_IsObject(callbackInfo_.thiz_); }

size_t Arguments::size() const { return callbackInfo_.argc_; }

Local<Value> Arguments::operator[](size_t i) const {
  if (i >= callbackInfo_.argc_) {
    return {};
  }
  return qjs_interop::makeLocal<Value>(
      qjs_backend::dupValue(callbackInfo_.argv_[i], callbackInfo_.engine_->context_));
}

ScriptEngine* Arguments::engine() const { return callbackInfo_.engine_; }

ScriptClass::ScriptClass(const script::Local<script::Object>& scriptObject) : internalState_() {
  internalState_.engine = &qjs_backend::currentEngine();
  // don't inc reference count, to pretend to be a weak ref
  // while they are not exactly weak ref as defined.
  // BUT, QuickJs calls finalize immediately, we can clear it there.
  // see QjsEngine::initEngineResource
  internalState_.weakRef_ = qjs_interop::peekLocal(scriptObject);
}

Local<Object> ScriptClass::getScriptObject() const {
  if (JS_IsObject(internalState_.weakRef_)) {
    return qjs_interop::makeLocal<Object>(
        qjs_backend::dupValue(internalState_.weakRef_, internalState_.engine->context_));
  }
  throw Exception("can't getScriptObject in finalizer");
}

Local<Array> ScriptClass::getInternalStore() const {
  if (JS_IsObject(internalState_.weakRef_)) {
    auto context = internalState_.engine->context_;
    JSAtom symbol = internalState_.engine->helperSymbolInternalStore_;
    auto obj = internalState_.weakRef_;
    auto store = JS_GetProperty(context, obj, symbol);
    qjs_backend::checkException(store);
    if (!JS_IsArray(context, store)) {
      JS_FreeValue(context, store);
      store = JS_NewArray(context);
      qjs_backend::checkException(store);
      qjs_backend::checkException(
          JS_SetProperty(context, obj, symbol, JS_DupValue(context, store)));
    }
    return qjs_interop::makeLocal<Array>(store);
  }
  throw Exception("can't getScriptObject in finalizer");
}

ScriptEngine* ScriptClass::getScriptEngine() const { return internalState_.engine; }

ScriptClass::~ScriptClass() = default;

}  // namespace script
