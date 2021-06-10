
#pragma once

#include "../../types.h"
#include "../../version.h"

// all in one header file
#include "../../Engine.h"
#include "../../Engine.hpp"
#include "../../Exception.h"
#include "../../Includes.h"
#include "../../Native.h"
#include "../../Native.hpp"
#include "../../Reference.h"
#include "../../Scope.h"
#include "../../Utils.h"
#include "../../Value.h"

#ifdef SCRIPTCORE_FEATURE_INSPECTOR
// inspector
#include "../../Inspector.h"
#endif

// utils
#include "../../utils/MessageQueue.h"
#include "../../utils/ThreadPool.h"

namespace script {

// export the implementation to script namespace
using ScriptEngineImpl = typename internal::ImplType<ScriptEngine>::type;

}  // namespace script

// undef internal used marcos
#undef StringLikeConcept
#undef StringLikeConceptCondition
#undef SCRIPTCORE_DISALLOW_COPY
#undef SCRIPTCORE_DISALLOW_MOVE
#undef SCRIPTCORE_DISALLOW_COPY_AND_MOVE
#undef SCRIPTCORE_DISALLOW_NEW
#undef SCRIPTCORE_UNUSED
#undef SCRIPTCORE_MARCO_TO_STRING
#undef SCRIPTCORE_MARCO_TO_STRING_INNER
#undef SCRIPTCORE_MARCO_JOIN
#undef SCRIPTCORE_MARCO_JOIN_INNER
#undef SCRIPTCORE_BACKEND
#undef SCRIPTCORE_BEGIN_INCLUDE_LIBRARY
#undef SCRIPTCORE_END_INCLUDE_LIBRARY
