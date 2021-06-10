
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include "../../../src/types.h"
#include "../WasmHelper.h"

namespace script {

namespace wasm_backend {

struct ByteBufferState {
  // val_ maybe any TypedArray or our "decorated shared Int8Array"
  mutable int val_ = -1;
  mutable std::shared_ptr<void> backingStore_;
  mutable size_t size_ = 0;
  mutable std::optional<ByteBuffer::Type> type_;

  /**
   * constructor to create ByteBuffer wrapping ArrayBuffer of SharedByteBuffer
   * @param val
   */
  /*no explict*/ ByteBufferState(int val);  // NOLINT

  /** constructor to create SharedByteBuffer */
  ByteBufferState(std::shared_ptr<void> backingStore, size_t size);

  ByteBufferState(ByteBufferState&& move) noexcept;
  ByteBufferState(const ByteBufferState&) = default;

  ByteBufferState& operator=(int);

  void reset() const;

  void fillTypeAndSize() const;

  void fillBackingStore() const;

  void fillWrapperIfNeed() const;
};

void swap(ByteBufferState& lhs, ByteBufferState& rhs);

}  // namespace wasm_backend

namespace internal {

template <typename T>
struct ImplType<Local<T>> {
  using type = int;
};

template <>
struct ImplType<Local<ByteBuffer>> {
  using type = wasm_backend::ByteBufferState;
};

template <typename T>
struct ImplType<Global<T>> {
  using type = int;
};

template <typename T>
struct ImplType<Weak<T>> {
  using type = int;
};

}  // namespace internal
}  // namespace script
