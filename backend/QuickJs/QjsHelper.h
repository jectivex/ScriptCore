
#pragma once
#include "../../src/foundation.h"
#include "../../src/types.h"

SCRIPTCORE_BEGIN_INCLUDE_LIBRARY
#include <quickjs.h>
SCRIPTCORE_END_INCLUDE_LIBRARY

namespace script::qjs_backend {
class QjsEngine;

JSContext* currentContext();

JSRuntime* currentRuntime();

QjsEngine& currentEngine();

/**
 * @param value  owned
 */
void checkException(JSValueConst value);

/**
 * throw if ret < 0
 * @param ret
 */
void checkException(int ret, const char* message = "Unknown Error");

JSValue throwException(const Exception& e, QjsEngine* engine = nullptr);

JSValue dupValue(JSValueConst val, JSContext* context = nullptr);

void freeValue(JSValueConst val, JSContext* context = nullptr);

}  // namespace script::qjs_backend
