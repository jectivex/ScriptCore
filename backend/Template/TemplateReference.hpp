
#pragma once
#include <utility>

namespace script {

template <typename T>
Global<T>::Global() noexcept : val_() {}

template <typename T>
Global<T>::Global(const script::Local<T>& localReference) {}

template <typename T>
Global<T>::Global(const script::Weak<T>& weak) {}

template <typename T>
Global<T>::Global(const script::Global<T>& copy) : val_(copy.val_) {}

template <typename T>
Global<T>::Global(script::Global<T>&& move) noexcept : val_(move.val_) {}

template <typename T>
Global<T>::~Global() {}

template <typename T>
Global<T>& Global<T>::operator=(const script::Global<T>& assign) {
  Global(assign).swap(*this);
  return *this;
}

template <typename T>
Global<T>& Global<T>::operator=(script::Global<T>&& move) noexcept {
  Global(std::move(move)).swap(*this);
  return *this;
}

template <typename T>
void Global<T>::swap(Global& rhs) noexcept {}

template <typename T>
Global<T>& Global<T>::operator=(const script::Local<T>& assign) {
  *this = Global<T>(assign);
  return *this;
}

template <typename T>
Local<T> Global<T>::get() const {
  TEMPLATE_NOT_IMPLEMENTED();
}

template <typename T>
Local<Value> Global<T>::getValue() const {
  TEMPLATE_NOT_IMPLEMENTED();
}

template <typename T>
bool Global<T>::isEmpty() const {
  return false;
}

template <typename T>
void Global<T>::reset() {}

// == Weak ==

template <typename T>
Weak<T>::Weak() noexcept : val_() {}

template <typename T>
Weak<T>::~Weak() {}

template <typename T>
Weak<T>::Weak(const script::Local<T>& localReference) {}

template <typename T>
Weak<T>::Weak(const script::Global<T>& globalReference) {}

template <typename T>
Weak<T>::Weak(const script::Weak<T>& copy) : val_(copy.val_) {}

template <typename T>
Weak<T>::Weak(script::Weak<T>&& move) noexcept : val_(std::move(move.val_)) {}

template <typename T>
Weak<T>& Weak<T>::operator=(const script::Weak<T>& assign) {
  val_ = assign.val_;
  return *this;
}

template <typename T>
Weak<T>& Weak<T>::operator=(script::Weak<T>&& move) noexcept {
  val_ = std::move(move.val_);
  return *this;
}

template <typename T>
void Weak<T>::swap(Weak& rhs) noexcept {
  std::swap(val_, rhs.val_);
}

template <typename T>
Weak<T>& Weak<T>::operator=(const script::Local<T>& assign) {
  *this = Weak<T>(assign);
  return *this;
}

template <typename T>
Local<T> Weak<T>::get() const {
  if (isEmpty()) throw Exception("get on empty Weak");
  TEMPLATE_NOT_IMPLEMENTED();
}

template <typename T>
Local<Value> Weak<T>::getValue() const {
  TEMPLATE_NOT_IMPLEMENTED();
}

template <typename T>
bool Weak<T>::isEmpty() const {
  return false;
}

template <typename T>
void Weak<T>::reset() noexcept {}

}  // namespace script
