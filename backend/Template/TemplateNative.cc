
#include <ScriptCore/ScriptCore.h>

namespace script {

Arguments::Arguments(InternalCallbackInfoType callbackInfo) : callbackInfo_(callbackInfo) {}

Arguments::~Arguments() = default;

Local<Object> Arguments::thiz() const { TEMPLATE_NOT_IMPLEMENTED(); }

bool Arguments::hasThiz() const { TEMPLATE_NOT_IMPLEMENTED(); }

size_t Arguments::size() const { TEMPLATE_NOT_IMPLEMENTED(); }

Local<Value> Arguments::operator[](size_t i) const { return {}; }

ScriptEngine* Arguments::engine() const { return nullptr; }

ScriptClass::ScriptClass(const script::Local<script::Object>& scriptObject) : internalState_() {
  TEMPLATE_NOT_IMPLEMENTED();
}

Local<Object> ScriptClass::getScriptObject() const { TEMPLATE_NOT_IMPLEMENTED(); }

Local<Array> ScriptClass::getInternalStore() const { TEMPLATE_NOT_IMPLEMENTED(); }

ScriptEngine* ScriptClass::getScriptEngine() const { TEMPLATE_NOT_IMPLEMENTED(); }

ScriptClass::~ScriptClass() = default;
}  // namespace script
