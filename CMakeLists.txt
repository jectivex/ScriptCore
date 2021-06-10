cmake_minimum_required(VERSION 3.0)

file(STRINGS ${CMAKE_CURRENT_LIST_DIR}/VERSION SCRIPTCORE_VERSION)

if ("${PROJECT_NAME}" STREQUAL "")
    project(ScriptCore VERSION ${SCRIPTCORE_VERSION} LANGUAGES CXX C)
endif ()

if ("${CMAKE_CXX_STANDARD}" STREQUAL "")
    message(WARNING "CMAKE_CXX_STANDARD not set, please set to 17 or latter")
endif ()

set(SCRIPTCORE_DIR ${CMAKE_CURRENT_LIST_DIR})

# supported targets
set(SCRIPTCORE_BACKEND_V8 V8)
set(SCRIPTCORE_BACKEND_JAVASCRIPTCORE JavaScriptCore)
set(SCRIPTCORE_BACKEND_SPIDERMONKEY SpiderMonkey)
set(SCRIPTCORE_BACKEND_QUICKJS QuickJs)
set(SCRIPTCORE_BACKEND_WEBASSEMBLY WebAssembly)
set(SCRIPTCORE_BACKEND_WKWEBVIEW WKWebView)
set(SCRIPTCORE_BACKEND_LUA Lua)
set(SCRIPTCORE_BACKEND_PYTHON Python)
set(SCRIPTCORE_BACKEND_RUBY Ruby)
# Empty is for you to compile against ScriptCore but don't keep any real implementations
set(SCRIPTCORE_BACKEND_EMPTY Empty)

set(SCRIPTCORE_BACKEND_LIST
        ${SCRIPTCORE_BACKEND_V8}
        ${SCRIPTCORE_BACKEND_JAVASCRIPTCORE}
        ${SCRIPTCORE_BACKEND_SPIDERMONKEY}
        ${SCRIPTCORE_BACKEND_QUICKJS}
        ${SCRIPTCORE_BACKEND_WEBASSEMBLY}
        ${SCRIPTCORE_BACKEND_WKWEBVIEW}
        ${SCRIPTCORE_BACKEND_LUA}
        ${SCRIPTCORE_BACKEND_PYTHON}
        ${SCRIPTCORE_BACKEND_RUBY}
        ${SCRIPTCORE_BACKEND_EMPTY}
        )

# set options, choose which ScriptCore Backend to use, V8 or JSC or etc...
set(SCRIPTCORE_BACKEND "" CACHE STRING "choose which ScriptCore Backend(Implementation) to use, V8 or JavaScriptCore or etc...")
set_property(CACHE SCRIPTCORE_BACKEND PROPERTY STRINGS "${SCRIPTCORE_BACKEND_LIST}")
option(SCRIPTCORE_NO_EXCEPTION_ON_BIND_FUNCTION "don't throw exception on defineClass generated bound function/get/set, return null & log instead. default to OFF" OFF)
option(SCRIPTCORE_FEATURE_INSPECTOR "enable inspector feature, default to OFF" OFF)

###### add ScriptCore library target ######

# define our target first
add_library(ScriptCore STATIC
        ${SCRIPTCORE_DIR}/src/Engine.h
        ${SCRIPTCORE_DIR}/src/Engine.hpp
        ${SCRIPTCORE_DIR}/src/Engine.cc
        ${SCRIPTCORE_DIR}/src/Reference.h
        ${SCRIPTCORE_DIR}/src/Reference.cc
        ${SCRIPTCORE_DIR}/src/Scope.h
        ${SCRIPTCORE_DIR}/src/Value.h
        ${SCRIPTCORE_DIR}/src/Exception.h
        ${SCRIPTCORE_DIR}/src/Inspector.h
        ${SCRIPTCORE_DIR}/src/types.h
        ${SCRIPTCORE_DIR}/src/Scope.cc
        ${SCRIPTCORE_DIR}/src/Utils.cc
        ${SCRIPTCORE_DIR}/src/utils/GlobalWeakBookkeeping.hpp
        ${SCRIPTCORE_DIR}/src/utils/Helper.hpp
        ${SCRIPTCORE_DIR}/src/utils/Helper.cc
        ${SCRIPTCORE_DIR}/src/utils/MemoryPool.hpp
        ${SCRIPTCORE_DIR}/src/utils/MessageQueue.cc
        ${SCRIPTCORE_DIR}/src/utils/ThreadPool.cc
        ${SCRIPTCORE_DIR}/src/utils/TypeInformation.h
        )

target_include_directories(ScriptCore PUBLIC ${SCRIPTCORE_DIR}/src/include)

###### config implementation ######

if ("${SCRIPTCORE_BACKEND}" STREQUAL "")
    message(FATAL_ERROR "Platform not set. Please set SCRIPTCORE_BACKEND. Candidates: ${SCRIPTCORE_BACKEND_LIST}")
endif ()

if (${SCRIPTCORE_BACKEND} STREQUAL ${SCRIPTCORE_BACKEND_V8})

    include(${SCRIPTCORE_DIR}/backend/V8/CMakeLists.txt)
    add_definitions(
            -DSCRIPTCORE_BACKEND_TRAIT_PREFIX=../backend/V8/trait/Trait
    )

elseif (${SCRIPTCORE_BACKEND} STREQUAL ${SCRIPTCORE_BACKEND_JAVASCRIPTCORE})

    include(${SCRIPTCORE_DIR}/backend/JavaScriptCore/CMakeLists.txt)
    add_definitions(
            -DSCRIPTCORE_BACKEND_TRAIT_PREFIX=../backend/JavaScriptCore/trait/Trait
    )

elseif (${SCRIPTCORE_BACKEND} STREQUAL ${SCRIPTCORE_BACKEND_SPIDERMONKEY})

    include(${SCRIPTCORE_DIR}/backend/SpiderMonkey/CMakeLists.txt)
    add_definitions(
            -DSCRIPTCORE_BACKEND_TRAIT_PREFIX=../backend/SpiderMonkey/trait/Trait
    )

elseif (${SCRIPTCORE_BACKEND} STREQUAL ${SCRIPTCORE_BACKEND_QUICKJS})

    include(${SCRIPTCORE_DIR}/backend/QuickJs/CMakeLists.txt)
    add_definitions(
            -DSCRIPTCORE_BACKEND_TRAIT_PREFIX=../backend/QuickJs/trait/Trait
    )

elseif (${SCRIPTCORE_BACKEND} STREQUAL ${SCRIPTCORE_BACKEND_WEBASSEMBLY})

    include(${SCRIPTCORE_DIR}/backend/WebAssembly/CMakeLists.txt)
    add_definitions(
            -DSCRIPTCORE_BACKEND_TRAIT_PREFIX=../backend/WebAssembly/trait/Trait
    )
    # see https://github.com/emscripten-core/emscripten/blob/master/src/settings.js
    # -sALLOW_MEMORY_GROWTH=1
    # -sINITIAL_MEMORY=167772160
    # -sUSE_PTHREADS=1
    target_compile_options(ScriptCore PRIVATE
            -sDISABLE_EXCEPTION_CATCHING=0
            )
    target_link_options(ScriptCore INTERFACE
            -sDISABLE_EXCEPTION_CATCHING=0
            -sLLD_REPORT_UNDEFINED
            )

elseif (${SCRIPTCORE_BACKEND} STREQUAL ${SCRIPTCORE_BACKEND_WKWEBVIEW})

    include(${SCRIPTCORE_DIR}/backend/WKWebView/CMakeLists.txt)
    add_definitions(
            -DSCRIPTCORE_BACKEND_TRAIT_PREFIX=../backend/WKWebView/trait/Trait
    )

elseif (${SCRIPTCORE_BACKEND} STREQUAL ${SCRIPTCORE_BACKEND_LUA})

    include(${SCRIPTCORE_DIR}/backend/Lua/CMakeLists.txt)
    add_definitions(
            -DSCRIPTCORE_BACKEND_TRAIT_PREFIX=../backend/Lua/trait/Trait
    )

elseif (${SCRIPTCORE_BACKEND} STREQUAL ${SCRIPTCORE_BACKEND_PYTHON})

    include(${SCRIPTCORE_DIR}/backend/Python/CMakeLists.txt)
    add_definitions(
            -DSCRIPTCORE_BACKEND_TRAIT_PREFIX=../backend/Python/trait/Trait
    )

elseif (${SCRIPTCORE_BACKEND} STREQUAL ${SCRIPTCORE_BACKEND_RUBY})

    include(${SCRIPTCORE_DIR}/backend/Ruby/CMakeLists.txt)
    add_definitions(
            -DSCRIPTCORE_BACKEND_TRAIT_PREFIX=../backend/Ruby/trait/Trait
    )

elseif (${SCRIPTCORE_BACKEND} STREQUAL ${SCRIPTCORE_BACKEND_EMPTY})

    include(${SCRIPTCORE_DIR}/backend/Template/CMakeLists.txt)
    add_definitions(
            -DSCRIPTCORE_BACKEND_TRAIT_PREFIX=../backend/Template/trait/Trait
    )

else ()

    message(FATAL_ERROR "platform not supported: ${SCRIPTCORE_BACKEND}. please set SCRIPTCORE_BACKEND. Candidates: ${SCRIPTCORE_BACKEND_LIST}")

endif ()

if (${SCRIPTCORE_NO_EXCEPTION_ON_BIND_FUNCTION})
    add_definitions(-DSCRIPTCORE_NO_EXCEPTION_ON_BIND_FUNCTION)
endif ()

if (${SCRIPTCORE_FEATURE_INSPECTOR})
    add_definitions(-DSCRIPTCORE_FEATURE_INSPECTOR)
endif ()

message(STATUS "Configuring ScriptCore version ${SCRIPTCORE_VERSION}.")
message(STATUS "Configuring ScriptCore using backend ${SCRIPTCORE_BACKEND}.")
message(STATUS "Configuring ScriptCore option SCRIPTCORE_NO_EXCEPTION_ON_BIND_FUNCTION ${SCRIPTCORE_NO_EXCEPTION_ON_BIND_FUNCTION}.")
message(STATUS "Configuring ScriptCore feature SCRIPTCORE_FEATURE_INSPECTOR ${SCRIPTCORE_FEATURE_INSPECTOR}.")

include(${SCRIPTCORE_DIR}/docs/doxygen/CMakeLists.txt)