
// this file is used only inside ScriptCore, no exported
#pragma once

#include <type_traits>
#include "../foundation.h"

// platform checks
#ifdef __ANDROID__

// other platform may also want to disable the use of thread_local
// add more checks after testing
#define SCRIPTCORE_DONT_USE_CPP_THREAD_LOCAL

#endif

#ifdef SCRIPTCORE_DONT_USE_CPP_THREAD_LOCAL
#include <pthread.h>
#include <cassert>
#endif

namespace script::internal {

// we leave the checks here, so that
// such restriction apply to all platform
template <typename T>
constexpr void checkThreadLocalType() {
  static_assert(std::is_default_constructible_v<T>);
  static_assert(std::is_destructible_v<T>);
}

#ifdef SCRIPTCORE_DONT_USE_CPP_THREAD_LOCAL

/**
 * A very simple wrapper around thread local
 * this class template must be used with static storage
 *
 * @tparam T must be default_constructible && destructible
 */
template <typename T>
class ThreadLocal {
 public:
  ThreadLocal();

  ~ThreadLocal();

  T& getValue();

 private:
  ::pthread_key_t threadLocalKey_{};
};

template <typename T>
ThreadLocal<T>::ThreadLocal() {
  checkThreadLocalType<T>();
  auto err =
      ::pthread_key_create(&threadLocalKey_, [](void* value) { delete static_cast<T*>(value); });
  // on Android NDK release build variant the assert marco expand to nothing
  // so we must mark "err" as used
  (void)err;
  assert(err == 0);

  err = ::pthread_setspecific(threadLocalKey_, new T{});
  assert(err == 0);
}

template <typename T>
ThreadLocal<T>::~ThreadLocal() {
  ::pthread_key_delete(threadLocalKey_);
}

template <typename T>
T& ThreadLocal<T>::getValue() {
  auto ptr = static_cast<T*>(::pthread_getspecific(threadLocalKey_));
  if (!ptr) {
    ptr = new T{};
    auto err = ::pthread_setspecific(threadLocalKey_, ptr);
    (void)err;
    assert(err == 0);
  }
  return *ptr;
}

#define SCRIPTCORE_THREAD_LOCAL(Type, Name) static ::script::internal::ThreadLocal<Type> Name

template <typename T>
static inline T& getThreadLocal(ThreadLocal<T>& tl) {
  return tl.getValue();
}

#else

#define SCRIPTCORE_THREAD_LOCAL(Type, Name) static thread_local Type Name

template <typename T>
inline T& getThreadLocal(T& v) {
  checkThreadLocalType<T>();
  return v;
}

#endif

}  // namespace script::internal

#undef SCRIPTCORE_DONT_USE_CPP_THREAD_LOCAL
