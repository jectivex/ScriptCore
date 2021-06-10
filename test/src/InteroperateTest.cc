
#include "test.h"

namespace script::test {

DEFINE_ENGINE_TEST(InteroperateTest);

#ifdef SCRIPTCORE_BACKEND_JAVASCRIPTCORE

TEST_F(InteroperateTest, JavaScriptCore) {
  EngineScope scope(engine);
  auto jsc = EngineScope::currentEngineAs<jsc_backend::JscEngine>();
  auto ctx = jsc_interop::getEngineContext(jsc);

  auto obj = Object::newObject();

  auto jobj = jsc_interop::toJsc(ctx, obj);
  EXPECT_TRUE(JSValueIsObject(ctx, jobj));

  auto jnum = JSValueMakeNumber(ctx, 3.14);
  auto num = jsc_interop::makeLocal<Number>(jnum);
  EXPECT_DOUBLE_EQ(num.toDouble(), 3.14);

  auto jstr = JSStringCreateWithUTF8CString("hello world");
  auto str = jsc_interop::makeLocal<String>(jstr);
  EXPECT_STREQ(str.toString().c_str(), "hello world");

  auto str2 = jsc_interop::makeLocal<String>(JSValueMakeString(ctx, jstr));
  EXPECT_STREQ(str2.toString().c_str(), "hello world");

  auto func = Function::newFunction([](const Arguments& args) -> Local<Value> {
    auto data = jsc_interop::extractArguments(args);
    auto ctx = jsc_interop::getEngineContext(data.engine);
    auto a = JSValueToNumber(ctx, data.arguments[0], nullptr);
    auto b = JSValueToNumber(ctx, data.arguments[1], nullptr);

    auto ret = JSValueMakeNumber(ctx, a + b);

    return jsc_interop::makeLocal<Number>(ret);
  });
  auto ret = func.call({}, 1, 2);
  ASSERT_TRUE(ret.isNumber());
  EXPECT_EQ(ret.asNumber().toInt32(), 3);
}

#endif

#ifdef SCRIPTCORE_BACKEND_V8

TEST_F(InteroperateTest, V8) {
  EngineScope scope(engine);
  auto v8eng = EngineScope::currentEngineAs<v8_backend::V8Engine>();
  auto iso = v8_interop::getEngineIsolate(v8eng);

  auto obj = Object::newObject();

  auto jobj = v8_interop::toV8<Object>(iso, obj);
  EXPECT_TRUE(jobj->IsObject());

  auto jnum = v8::Number::New(iso, 3.14);
  auto num = v8_interop::makeLocal<Number>(jnum);
  EXPECT_DOUBLE_EQ(num.toDouble(), 3.14);

  auto jstr =
      v8::String::NewFromUtf8(iso, "hello world", v8::NewStringType::kNormal).ToLocalChecked();
  auto str = v8_interop::makeLocal<String>(jstr);
  EXPECT_STREQ(str.toString().c_str(), "hello world");

  auto func = Function::newFunction([](const Arguments& args) -> Local<Value> {
    auto data = v8_interop::extractArguments(args);
    auto iso = v8_interop::getEngineIsolate(data.engine);
    auto ctx = v8_interop::getEngineContext(data.engine);

    auto a = data.args[0]->ToNumber(ctx).ToLocalChecked()->Value();
    auto b = data.args[1]->ToNumber(ctx).ToLocalChecked()->Value();

    auto ret = v8::Number::New(iso, a + b);

    // use the function return value，don't use v8::FunctionCallbackInfo::GetReturnValue
    return v8_interop::makeLocal<Number>(ret);
  });
  auto ret = func.call({}, 1, 2);
  ASSERT_TRUE(ret.isNumber());
  EXPECT_EQ(ret.asNumber().toInt32(), 3);
}

#endif

#ifdef SCRIPTCORE_BACKEND_LUA

TEST_F(InteroperateTest, Lua) {
  EngineScope scope(engine);
  auto luaEng = EngineScope::currentEngineAs<lua_backend::LuaEngine>();
  auto lua = lua_interop::getEngineLua(luaEng);

  auto obj = Object::newObject();
  auto jobj = lua_interop::toLua<Object>(obj);
  EXPECT_TRUE(lua_istable(lua, jobj));

  lua_pushnumber(lua, 3.14);
  auto lnum = lua_gettop(lua);
  auto num = lua_interop::makeLocal<Number>(lnum);
  EXPECT_DOUBLE_EQ(num.toDouble(), 3.14);

  lua_pushstring(lua, "hello world");
  auto lstr = lua_gettop(lua);
  auto str = lua_interop::makeLocal<String>(lstr);
  EXPECT_STREQ(str.toString().c_str(), "hello world");

  auto func = Function::newFunction([](const Arguments& args) -> Local<Value> {
    auto data = lua_interop::extractArguments(args);
    auto lua = lua_interop::getEngineLua(data.engine);
    auto a = lua_tonumber(lua, data.stackBase);
    auto b = lua_tonumber(lua, data.stackBase + 1);

    lua_pushnumber(lua, a + b);
    auto ret = lua_gettop(lua);

    return lua_interop::makeLocal<Number>(ret);
  });
  auto ret = func.call({}, 1, 2);
  ASSERT_TRUE(ret.isNumber());
  EXPECT_EQ(ret.asNumber().toInt32(), 3);
}

#endif

}  // namespace script::test
