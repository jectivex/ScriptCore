
#include "test.h"
// include private class
#include "../../src/utils/ThreadLocal.h"

namespace script::test {

DEFINE_ENGINE_TEST(EngineScopeTest);

TEST_F(EngineScopeTest, CurrentEngine) {
  EXPECT_EQ(script::EngineScope::currentEngine(), nullptr);

  {
    script::EngineScope engineScope(engine);
    EXPECT_EQ(script::EngineScope::currentEngine(), engine);
    {
      script::EngineScope engineScope2(engine);
      EXPECT_EQ(script::EngineScope::currentEngine(), engine);
    }
    EXPECT_EQ(script::EngineScope::currentEngine(), engine);
  }

  EXPECT_EQ(script::EngineScope::currentEngine(), nullptr);
}

TEST_F(EngineScopeTest, ExitEngine) {
  EXPECT_EQ(script::EngineScope::currentEngine(), nullptr);

  {
    script::EngineScope engineScope(engine);
    EXPECT_EQ(script::EngineScope::currentEngine(), engine);
    {
      script::ExitEngineScope exit;
      EXPECT_EQ(script::EngineScope::currentEngine(), nullptr);

      std::thread t([&]() { script::EngineScope engineScope2(engine); });

      // thread t should be able to obtain another scope
      // and run normally
      // shouldn't wait on lock
      t.join();
    }
    EXPECT_EQ(script::EngineScope::currentEngine(), engine);
  }

  EXPECT_EQ(script::EngineScope::currentEngine(), nullptr);
}

TEST_F(EngineScopeTest, ReenterExit) {
  EXPECT_EQ(script::EngineScope::currentEngine(), nullptr);
#ifdef SCRIPTCORE_BACKEND_WEBASSEMBLY
  auto* newEngine = script::ScriptEngineImpl::instance();
#else
  auto* newEngine = new script::ScriptEngineImpl();
#endif

  {
    script::EngineScope engineScope(engine);
    EXPECT_EQ(script::EngineScope::currentEngine(), engine);
    {
      // 可重入
      script::EngineScope engineScope2(engine);
      EXPECT_EQ(script::EngineScope::currentEngine(), engine);

      {
        // 可交错
        script::EngineScope another(newEngine);
        EXPECT_EQ(script::EngineScope::currentEngine(), newEngine);

        {
          // 交错重入
          script::EngineScope engineScopeAgain(engine);
          EXPECT_EQ(script::EngineScope::currentEngine(), engine);

          {
            // 可临时退出
            script::ExitEngineScope exit;
            EXPECT_EQ(script::EngineScope::currentEngine(), nullptr);

            {
              // 疯狂重入
              script::EngineScope another2(newEngine);
              EXPECT_EQ(script::EngineScope::currentEngine(), newEngine);
            }
          }
        }
      }

      EXPECT_EQ(script::EngineScope::currentEngine(), engine);
    }
    EXPECT_EQ(script::EngineScope::currentEngine(), engine);
  }

  EXPECT_EQ(script::EngineScope::currentEngine(), nullptr);

  newEngine->destroy();
}

TEST_F(EngineScopeTest, StackFrameScope) {
  using namespace script;
  EngineScope engineScope(engine);
  Local<Value> val;
  {
    StackFrameScope stack;
    auto s = String::newString("InsideStack");
    val = stack.returnValue(s);
  }

  ASSERT_TRUE(val.isString());

  EXPECT_STREQ(val.asString().toString().c_str(), "InsideStack");
}

TEST_F(EngineScopeTest, TwoThreads) {
  EXPECT_EQ(script::EngineScope::currentEngine(), nullptr);

  {
    script::EngineScope engineScope(engine);
    EXPECT_EQ(script::EngineScope::currentEngine(), engine);
    {
      std::thread t([]() { EXPECT_EQ(script::EngineScope::currentEngine(), nullptr); });
      t.join();
    }
    EXPECT_EQ(script::EngineScope::currentEngine(), engine);
  }

  EXPECT_EQ(script::EngineScope::currentEngine(), nullptr);
}

TEST_F(EngineScopeTest, ThreadLocal) {
  SCRIPTCORE_THREAD_LOCAL(int, threadLocal_1);
  SCRIPTCORE_THREAD_LOCAL(int, threadLocal_2);

  script::internal::getThreadLocal(threadLocal_1) = 0;
  script::internal::getThreadLocal(threadLocal_2) = 0;

  EXPECT_EQ(script::internal::getThreadLocal(threadLocal_1), 0);
  EXPECT_EQ(script::internal::getThreadLocal(threadLocal_2), 0);

  script::internal::getThreadLocal(threadLocal_1) = 1;

  EXPECT_EQ(script::internal::getThreadLocal(threadLocal_1), 1);
  EXPECT_EQ(script::internal::getThreadLocal(threadLocal_2), 0);

  {
    std::thread t([]() {
      EXPECT_EQ(script::internal::getThreadLocal(threadLocal_1), 0);
      script::internal::getThreadLocal(threadLocal_1) = 42;
      EXPECT_EQ(script::internal::getThreadLocal(threadLocal_1), 42);
    });
    t.join();
  }

  EXPECT_EQ(script::internal::getThreadLocal(threadLocal_1), 1);
  EXPECT_EQ(script::internal::getThreadLocal(threadLocal_2), 0);
}

}  // namespace script::test
