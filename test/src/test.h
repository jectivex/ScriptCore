
#pragma once

#include <gtest/gtest.h>

#include <ScriptCore/ScriptCore.h>

#ifndef __cpp_lib_char8_t
using char8_t = char;
#endif

namespace script::test {
// TestScript
struct TS {
  template <typename T>
  TS& js(T&& s) {
    static_cast<void>(s);
#ifdef SCRIPTCORE_LANG_JAVASCRIPT
    script = script::String::newString(std::forward<T>(s));
#endif
    return *this;
  }

  template <typename T>
  TS& lua(T&& s) {
    static_cast<void>(s);
#ifdef SCRIPTCORE_LANG_LUA
    script = script::String::newString(std::forward<T>(s));
#endif
    return *this;
  }

  script::Local<script::String> select() {
    if (script.isNull()) {
      throw std::runtime_error("add script for current language");
    }
    return script.asString();
  }

 private:
  script::Local<script::Value> script;
};

class ScriptCoreTestFixture : public testing::Test {
 protected:
  script::ScriptEngine* engine = nullptr;

  void SetUp() override;

  void TearDown() override;

  void destroyEngine();
};
}  // namespace script::test

#define DEFINE_ENGINE_TEST(NAME) \
  class NAME : public ::script::test::ScriptCoreTestFixture {}
