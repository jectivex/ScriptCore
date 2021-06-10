
#include <iostream>
#include "test.h"

namespace {
class Logger : public script::Logger::Delegate {
 public:
  void log(const char* msg) override { std::cout << msg << std::endl; }
};

Logger logger;

script::Local<script::Value> consoleLog(const script::Arguments& args) {
  for (size_t i = 0; i < args.size(); ++i) {
    if (args[i].isString()) {
      std::cout << args[i].asString().toString().c_str();
    } else {
      std::cout << args[i].describeUtf8();
    }
    if (i + 1 < args.size()) {
      std::cout << " ";
    }
  }
  std::cout << std::endl;
  return {};
}

}  // namespace

namespace script::test {

void ScriptCoreTestFixture::SetUp() {
#ifdef SCRIPTCORE_BACKEND_WEBASSEMBLY
  auto eng = script::ScriptEngineImpl::instance();
  script::ScriptEngineImpl::ignoreDestroyCall();
  eng->unitTestResetRetistry();
  engine = eng;
#else
  engine = new script::ScriptEngineImpl();
#endif
  {
    using script::EngineScope;
    using script::Function;
    using script::Object;
    using script::String;

    EngineScope engineScope(engine);
    auto log = Function::newFunction(consoleLog);
#ifndef SCRIPTCORE_BACKEND_WEBASSEMBLY
    auto console = Object::newObject();
    console.set(String::newString(u8"log"), log);
    engine->set(String::newString(u8"console"), console);
#endif
    engine->set(String::newString(u8"print"), log);
  }
}

void ScriptCoreTestFixture::TearDown() {
  if (engine) engine->destroy();
}

void ScriptCoreTestFixture::destroyEngine() {
  engine->destroy();
  engine = nullptr;
}

}  // namespace script::test

void initExternalData(int argc, char** argv) {
#ifdef SCRIPTCORE_BACKEND_V8
  v8::V8::InitializeExternalStartupData(argv[0]);
#endif
}

GTEST_API_ int main(int argc, char** argv) {
  printf("Running main() from %s\n", __FILE__);
  initExternalData(argc, argv);
  script::Logger::setDelegate(&logger);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
