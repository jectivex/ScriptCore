
#include "../../src/Exception.h"
#include "../../src/Reference.h"
#include "../../src/Scope.h"
#include "../../src/Value.h"

namespace script {

Local<Object> Object::newObject() { TEMPLATE_NOT_IMPLEMENTED(); }

Local<Object> Object::newObjectImpl(const Local<Value>& type, size_t size,
                                    const Local<Value>* args) {
  TEMPLATE_NOT_IMPLEMENTED();
}

Local<String> String::newString(const char* utf8) { TEMPLATE_NOT_IMPLEMENTED(); }

Local<String> String::newString(std::string_view utf8) { TEMPLATE_NOT_IMPLEMENTED(); }

Local<String> String::newString(const std::string& utf8) { TEMPLATE_NOT_IMPLEMENTED(); }

#if defined(__cpp_char8_t)

Local<String> String::newString(const char8_t* utf8) {
  return newString(reinterpret_cast<const char*>(utf8));
}

Local<String> String::newString(std::u8string_view utf8) {
  return newString(std::string_view(reinterpret_cast<const char*>(utf8.data()), utf8.length()));
}

Local<String> String::newString(const std::u8string& utf8) { return newString(utf8.c_str()); }

#endif

Local<Number> Number::newNumber(float value) { return newNumber(static_cast<double>(value)); }

Local<Number> Number::newNumber(double value) { TEMPLATE_NOT_IMPLEMENTED(); }

Local<Number> Number::newNumber(int32_t value) { return newNumber(static_cast<double>(value)); }

Local<Number> Number::newNumber(int64_t value) { return newNumber(static_cast<double>(value)); }

Local<Boolean> Boolean::newBoolean(bool value) { TEMPLATE_NOT_IMPLEMENTED(); }

Local<Function> Function::newFunction(script::FunctionCallback callback) {
  TEMPLATE_NOT_IMPLEMENTED();
}

Local<Array> Array::newArray(size_t size) { TEMPLATE_NOT_IMPLEMENTED(); }

Local<Array> Array::newArrayImpl(size_t size, const Local<Value>* args) {
  TEMPLATE_NOT_IMPLEMENTED();
}

Local<ByteBuffer> ByteBuffer::newByteBuffer(size_t size) { TEMPLATE_NOT_IMPLEMENTED(); }

Local<script::ByteBuffer> ByteBuffer::newByteBuffer(void* nativeBuffer, size_t size) {
  TEMPLATE_NOT_IMPLEMENTED();
}

Local<ByteBuffer> ByteBuffer::newByteBuffer(std::shared_ptr<void> nativeBuffer, size_t size) {
  TEMPLATE_NOT_IMPLEMENTED();
}

}  // namespace script
