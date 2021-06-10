
#pragma once
#include "LuaEngine.h"

namespace script::lua_backend {

class LuaByteBufferImpl : public LuaByteBufferDelegate {
 public:
  LuaByteBufferImpl() = default;

  SCRIPTCORE_DISALLOW_COPY_AND_MOVE(LuaByteBufferImpl);

  ~LuaByteBufferImpl() override = default;

  void init(LuaEngine *engine) override;

  Local<Value> newByteBuffer(LuaEngine *engine, std::shared_ptr<void> nativeBuffer,
                             size_t size) override;

  std::shared_ptr<void> getByteBuffer(LuaEngine *engine, const Local<Value> &byteBuffer) override;

  size_t getByteBufferSize(LuaEngine *engine, const Local<Value> &byteBuffer) override;

  bool isByteBuffer(LuaEngine *engine, const Local<Value> &byteBuffer) override;
};

}  // namespace script::lua_backend
