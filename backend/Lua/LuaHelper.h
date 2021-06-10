
#pragma once

#include "../../src/types.h"

SCRIPTCORE_BEGIN_INCLUDE_LIBRARY
#include <lua.hpp>
SCRIPTCORE_END_INCLUDE_LIBRARY

namespace script::lua_backend {

constexpr const char* kLuaGlobalEnvName = "_G";
constexpr const char* kLuaMetaMethodMode = "__mode";
constexpr const char* kLuaMetaMethodIndex = "__index";
constexpr const char* kLuaMetaMethodNewIndex = "__newindex";
constexpr const char* kLuaMetaMethodCall = "__call";
constexpr const char* kLuaMetaMethodNewGc = "__gc";

Local<Value> callFunction(const Local<Value>& func, const Local<Value>& thiz, size_t argsCount,
                          const Local<Value>* begin);

int handleReturnToLua(lua_State* lua, int retIndex);

// [0, +1, -]
int pushFinalizableUserData(lua_State* lua, void* data, void finalizer(void*));

// [+1, 0, -]
void* getUserDataFromFinalizableUserData(lua_State* lua, int finalizableUserDataIndex = -1);

void luaCopyTable(lua_State* lua, int from, int to);

Local<Object> luaNewObject(const Local<Value>& obj, size_t size, const Local<Value>* ptr);

}  // namespace script::lua_backend

namespace script {
struct lua_interop;
}
