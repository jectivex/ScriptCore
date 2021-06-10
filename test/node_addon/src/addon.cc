
#include <ScriptCore/ScriptCore.h>
#include <node.h>
#include <iostream>

namespace script {

namespace binding {

const ClassDefine<void>& hello() {
  static auto define =
      defineClass("ScriptCoreDemoTest").function("hello", []() { return "world"; }).build();
  return define;
}

}  // namespace binding

void scheduleMessageQueue(ScriptEngine* engine);

void Initialize(v8::Local<v8::Object> /*exports*/, v8::Local<v8::Value> /*module*/,
                v8::Local<v8::Context> context, void* /*priv*/) {
  auto isolate = v8::Isolate::GetCurrent();
  // 1. create script engine
  ScriptEngine* engine = new ScriptEngineImpl({}, isolate, context);

  std::cout << ">> Initialize ScriptEngine for node.js [" << engine << "]" << std::endl;

  {
    // 2. init script engine
    EngineScope enter(engine);
    engine->registerNativeClass(binding::hello());

    // 3.
    //    schedule loop message queue as need
    //    1. just loop it
    //    scheduleMessageQueue(engine);
    //
    //    2. call loop once in your periodic function
  }

  // 4. called before isolate destroy
  node::AddEnvironmentCleanupHook(
      isolate,
      [](void* arg) {
        static_cast<ScriptEngine*>(arg)->destroy();

        std::cout << ">> Destroy ScriptEngine for node.js [" << arg << "]" << std::endl;
      },
      engine);
}

void scheduleMessageQueue(ScriptEngine* engine) {
  engine->get("setInterval")
      .asFunction()
      .call({}, Function::newFunction([](const Arguments& args) {
              args.engine()->messageQueue()->loopQueue(utils::MessageQueue::LoopType::kLoopOnce);
              return Local<Value>{};
            }),
            16);
}

NODE_MODULE_CONTEXT_AWARE(ScriptEngineNodeAddon, Initialize)

}  // namespace script
