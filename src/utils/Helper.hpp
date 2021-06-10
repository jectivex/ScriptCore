
#pragma once
#include <utility>
#include <vector>
#include "../Reference.h"
#include "../foundation.h"

namespace script::internal {

namespace detail {

template <typename T, size_t N, typename FN>
bool withNArrayHelper(FN&& fn) {
  T array[N];
  fn(array);
  return true;
}

constexpr size_t powOf2(size_t n) { return n == 0 ? 1 : powOf2(n - 1) * 2; }

template <typename T, typename FN, size_t... I>
void withNArrayHelper2(FN&& fn, size_t n, std::index_sequence<I...>) {
  (... ||
   (n <= detail::powOf2(I) && withNArrayHelper<T, detail::powOf2(I), FN>(std::forward<FN>(fn))));
}

}  // namespace detail

template <typename T, typename FN>
void withNArray(size_t N, FN&& fn) {
  constexpr size_t kMax = 6;
  if (N < detail::powOf2(kMax)) {
    detail::withNArrayHelper2<T>(std::forward<FN>(fn), N, std::make_index_sequence<kMax + 1>());
  } else {
    std::vector<T> array(N);
    fn(array.data());
  }
}

Local<Value> getNamespaceObject(ScriptEngine* engine, const std::string_view& nameSpace,
                                Local<Value> rootNs = {});
}  // namespace script::internal
