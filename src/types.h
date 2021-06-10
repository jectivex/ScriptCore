
#pragma once

#include <functional>
#include "foundation.h"

// all exported types

namespace script {

// ==== engine ====
class ScriptEngine;

// ==== scope ====
class EngineScope;

class ExitEngineScope;

class StackFrameScope;

// ==== reference ====
template <typename T>
class Local;

template <typename T>
class Global;

template <typename T>
class Weak;

// forward declared specialized local types

// ==== value ====

enum class ValueKind;

class Value;

class Object;

class String;

class Number;

class Boolean;

class Function;

class Array;

class ByteBuffer;

class Unsupported;

// ==== exception ====

class Exception;

// ==== native ====
class Arguments;

using FunctionCallback = std::function<Local<Value>(const Arguments& args)>;
using GetterCallback = std::function<Local<Value>()>;
using SetterCallback = std::function<void(const Local<Value>& value)>;

template <typename T>
using InstanceFunctionCallback = std::function<Local<Value>(T*, const Arguments& args)>;

template <typename T>
using InstanceSetterCallback = std::function<void(T*, const Local<Value>& value)>;

template <typename T>
using InstanceGetterCallback = std::function<Local<Value>(T*)>;

template <typename T>
using InstanceConstructor = std::function<T*(const Arguments& args)>;

class ScriptClass;

template <typename T = void>
class ClassDefine;

class ClassDefineVisitor;

/**
 * for method has a param <b>nothrow:</b> when arguments type mismatch, throw the cast Exception or
 * just return null. note: the exception thrown by the callback still got propagated.
 */
template <typename T = void>
class ClassDefineBuilder;

class NativeRegister;

// ==== inspector ====
#ifdef SCRIPTCORE_FEATURE_INSPECTOR
class InspectorAgent;

class ScriptInspector;
#endif

// ==== utils ====
class StringHolder;

enum class ScriptLanguage;

class Tracer;

// ==== C++ 20 concepts for StringLike ====

// StringLike is of type
// 1. char*
// 2. string
// 3. string_view
// 4. char8_t
// 5. u8string
// 6. u8string_view
// The call to String::newString(StringLike) must be valid
// ie. String must implement above overloads.
// We don't have C++ 20 concept ready, so just use a marco to simulate
// using SFINA technology.

// android ndk can't compile follow declare until NDK r21
// #define StringLikeConcept(StringLike)
//   typename = std::void_t<decltype(::script::String::newString(std::declval<StringLike>()))>

#ifdef __cpp_lib_char8_t

#define StringLikeConceptCondition(StringLike)                        \
  std::is_convertible_v<StringLike, const char*> ||                   \
      std::is_convertible_v<StringLike, const std::string_view&> ||   \
      std::is_convertible_v<StringLike, const std::string&> ||        \
      std::is_convertible_v<StringLike, const char8_t*> ||            \
      std::is_convertible_v<StringLike, const std::u8string_view&> || \
      std::is_convertible_v<StringLike, const std::u8string&>

#else

#define StringLikeConceptCondition(StringLike)                      \
  std::is_convertible_v<StringLike, const char*> ||                 \
      std::is_convertible_v<StringLike, const std::string_view&> || \
      std::is_convertible_v<StringLike, const std::string&>

#endif

#define StringLikeConcept(StringLike) \
  typename = std::enable_if_t<StringLikeConceptCondition(StringLike)>

// ==== internal ====
namespace internal {

class StaticDefine;

template <typename T, typename>
struct TypeHolder;

struct interop {};
}  // namespace internal

}  // namespace script
