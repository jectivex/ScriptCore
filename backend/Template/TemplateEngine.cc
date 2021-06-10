
#include "TemplateEngine.h"
#include "../../src/Utils.h"

namespace script::template_backend {

TemplateEngine::TemplateEngine(std::shared_ptr<utils::MessageQueue> queue) {}

TemplateEngine::TemplateEngine() : TemplateEngine(std::shared_ptr<utils::MessageQueue>{}) {}

TemplateEngine::~TemplateEngine() = default;

void TemplateEngine::destroy() noexcept {}

Local<Value> TemplateEngine::get(const Local<String>& key) { return Local<Value>(); }

void TemplateEngine::set(const Local<String>& key, const Local<Value>& value) {}

Local<Value> TemplateEngine::eval(const Local<String>& script) {
  return eval(script, Local<Value>());
}

Local<Value> TemplateEngine::eval(const Local<String>& script, const Local<String>& sourceFile) {
  return eval(script, sourceFile.asValue());
}

Local<Value> TemplateEngine::eval(const Local<String>& script, const Local<Value>& sourceFile) {
  return Local<Value>();
}

std::shared_ptr<utils::MessageQueue> TemplateEngine::messageQueue() {
  return std::shared_ptr<utils::MessageQueue>();
}

void TemplateEngine::gc() {}

void TemplateEngine::adjustAssociatedMemory(int64_t count) {}

ScriptLanguage TemplateEngine::getLanguageType() { return ScriptLanguage::kJavaScript; }

std::string TemplateEngine::getEngineVersion() { return ""; }

bool TemplateEngine::isDestroying() const { return false; }

}  // namespace script::template_backend
