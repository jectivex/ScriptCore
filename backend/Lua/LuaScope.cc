
#include "LuaScope.hpp"
#include "trait/TraitScope.h"

namespace script::lua_backend {

StackFrameScopeImpl::StackFrameScopeImpl(LuaEngine &engine) : engine_(&engine) {
  pushValue(engine_->lua_, {});
  base_ = lua_gettop(engine_->lua_);
}

int StackFrameScopeImpl::returnRawValue(int val) {
  auto lua = engine_->lua_;
  auto returnIndex = base_;
  if (val == 0) {
    lua_pushnil(lua);
  } else {
    lua_pushvalue(lua, val);
  }
  lua_replace(lua, returnIndex);
  hasReturnValue_ = true;
  return returnIndex;
}

StackFrameScopeImpl::~StackFrameScopeImpl() {
  if (hasReturnValue_) {
    lua_settop(engine_->lua_, base_);
  } else {
    lua_settop(engine_->lua_, base_ - 1);
  }
}

EngineScopeImpl::EngineScopeImpl(LuaEngine &engine) : lockGuard_(engine.lock_), stack_(engine) {
  auto prevEngine = EngineScope::currentEngineAs<LuaEngine>();
  if (prevEngine && prevEngine != stack_.engine_) {
    prevEngine_ = prevEngine;
    prevEngine->lock_.unlock();
  }
}

EngineScopeImpl::~EngineScopeImpl() {
  if (prevEngine_) {
    prevEngine_->lock_.lock();
  }
}

ExitEngineScopeImpl::ExitEngineScopeImpl(LuaEngine &engine) : engine_(&engine) {
  engine_->lock_.unlock();
}

ExitEngineScopeImpl::~ExitEngineScopeImpl() { engine_->lock_.lock(); }

}  // namespace script::lua_backend
