#### config fields

# default location is next to ScriptCore root dir
# ScriptCoreLibs download by cmake script
if ("${SCRIPTCORE_TEST_LIBS}" STREQUAL "")
    if (NOT "$ENV{SCRIPTCORE_TEST_LIBS}" STREQUAL "")
        set(SCRIPTCORE_TEST_LIBS "$ENV{SCRIPTCORE_TEST_LIBS}")
    else ()
        set(SCRIPTCORE_TEST_LIBS ${CMAKE_CURRENT_BINARY_DIR}/ScriptCoreTestLibs)
    endif ()
endif ()

if ("${SCRIPTCORE_TEST_GOOGLE_TEST}" STREQUAL "")
    if (NOT "$ENV{SCRIPTCORE_TEST_GOOGLE_TEST}" STREQUAL "")
        set(SCRIPTCORE_TEST_GOOGLE_TEST "$ENV{SCRIPTCORE_TEST_GOOGLE_TEST}")
    else ()
        set(SCRIPTCORE_TEST_GOOGLE_TEST ${CMAKE_CURRENT_BINARY_DIR}/googletest-src)
    endif ()
endif ()

if ("${SCRIPTCORE_TEST_BUILD_ONLY}" STREQUAL "")
    if (NOT "$ENV{SCRIPTCORE_TEST_BUILD_ONLY}" STREQUAL "")
        set(SCRIPTCORE_TEST_BUILD_ONLY "$ENV{SCRIPTCORE_TEST_BUILD_ONLY}")
    else ()
        set(SCRIPTCORE_TEST_BUILD_ONLY OFF)
    endif ()
endif ()

# set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_BUILD_TYPE RelWithDebInfo)

# set(TEST_FLAG_ENABLE_CLANG_TIDY ON)
# set(TEST_FLAG_ENABLE_ASAN ON)
# set(TEST_FLAG_ENABLE_UBSAN ON)

if ("${SCRIPTCORE_BACKEND}" STREQUAL "")
    ### choose your backend
    set(SCRIPTCORE_BACKEND V8 CACHE STRING "" FORCE)
    #set(SCRIPTCORE_BACKEND JavaScriptCore CACHE STRING "" FORCE)
    #set(SCRIPTCORE_BACKEND Lua CACHE STRING "" FORCE)
    #set(SCRIPTCORE_BACKEND WebAssembly CACHE STRING "" FORCE)
    #set(SCRIPTCORE_BACKEND QuickJs CACHE STRING "" FORCE)
    #set(SCRIPTCORE_BACKEND Empty CACHE STRING "" FORCE)
endif ()

# wasm need emscripten toolchain file, like
# -DCMAKE_TOOLCHAIN_FILE=<emsdk>/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
# install using https://github.com/emscripten-core/emsdk

# download test libs if need
include(${CMAKE_CURRENT_LIST_DIR}/test_libs/CMakeLists.txt)

if (${SCRIPTCORE_BACKEND} STREQUAL V8)
    if (SCRIPTCORE_TEST_BUILD_ONLY)
        set(DEVOPS_LIBS_INCLUDE
                "${SCRIPTCORE_TEST_LIBS}/mac/v8/include"
                CACHE STRING "" FORCE)
    elseif (APPLE)
        set(DEVOPS_LIBS_INCLUDE
                "${SCRIPTCORE_TEST_LIBS}/mac/v8/include"
                CACHE STRING "" FORCE)
        set(DEVOPS_LIBS_LIBPATH
                "${SCRIPTCORE_TEST_LIBS}/mac/v8/libv8_monolith.a"
                CACHE STRING "" FORCE)

    elseif (WIN32)
        set(DEVOPS_LIBS_INCLUDE
                "${SCRIPTCORE_TEST_LIBS}/win64/v8/include"
                CACHE STRING "" FORCE)

        set(DEVOPS_LIBS_LIBPATH
                "${SCRIPTCORE_TEST_LIBS}/win64/v8/v8_libbase.dll.lib"
                "${SCRIPTCORE_TEST_LIBS}/win64/v8/v8_libplatform.dll.lib"
                "${SCRIPTCORE_TEST_LIBS}/win64/v8/v8.dll.lib"
                CACHE STRING "" FORCE)

        add_custom_command(TARGET UnitTests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${SCRIPTCORE_TEST_LIBS}/win64/v8/dll" $<TARGET_FILE_DIR:UnitTests>
                )

    endif ()
elseif (${SCRIPTCORE_BACKEND} STREQUAL JavaScriptCore)
    if (SCRIPTCORE_TEST_BUILD_ONLY)
        set(DEVOPS_LIBS_INCLUDE
                "${SCRIPTCORE_TEST_LIBS}/win64/jsc/include"
                CACHE STRING "" FORCE)
    elseif (APPLE)
        set(DEVOPS_LIBS_INCLUDE
                ""
                CACHE STRING "" FORCE)

        set(DEVOPS_LIBS_LIBPATH
                "-w -framework Foundation -framework JavaScriptCore"
                CACHE STRING "" FORCE)
    elseif (WIN32)
        set(DEVOPS_LIBS_INCLUDE
                "${SCRIPTCORE_TEST_LIBS}/win64/jsc/include"
                CACHE STRING "" FORCE)

        set(DEVOPS_LIBS_LIBPATH
                "${SCRIPTCORE_TEST_LIBS}/win64/jsc/JavaScriptCore.lib"
                CACHE STRING "" FORCE)

        add_custom_command(TARGET UnitTests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${SCRIPTCORE_TEST_LIBS}/win64/jsc/dll" $<TARGET_FILE_DIR:UnitTests>
                )
    endif ()

elseif (${SCRIPTCORE_BACKEND} STREQUAL Lua)
    if (SCRIPTCORE_TEST_BUILD_ONLY)
        set(DEVOPS_LIBS_INCLUDE
                "${SCRIPTCORE_TEST_LIBS}/mac/lua/include"
                CACHE STRING "" FORCE)
    elseif (APPLE)
        set(DEVOPS_LIBS_INCLUDE
                "${SCRIPTCORE_TEST_LIBS}/mac/lua/include"
                CACHE STRING "" FORCE)
        set(DEVOPS_LIBS_LIBPATH
                "${SCRIPTCORE_TEST_LIBS}/mac/lua/liblua.a"
                CACHE STRING "" FORCE)
    elseif (WIN32)
        set(DEVOPS_LIBS_INCLUDE
                "${SCRIPTCORE_TEST_LIBS}/win64/lua/include"
                CACHE STRING "" FORCE)

        set(DEVOPS_LIBS_LIBPATH
                "${SCRIPTCORE_TEST_LIBS}/win64/lua/Lua54.lib"
                CACHE STRING "" FORCE)
    endif ()

elseif (${SCRIPTCORE_BACKEND} STREQUAL WebAssembly)
    if ("${CMAKE_TOOLCHAIN_FILE}" STREQUAL "")
        message(FATAL_ERROR "CMAKE_TOOLCHAIN_FILE must be passed for emscripten")
    endif ()
elseif (${SCRIPTCORE_BACKEND} STREQUAL QuickJs)
    include("${SCRIPTCORE_TEST_LIBS}/quickjs/CMakeLists.txt")
    set(DEVOPS_LIBS_LIBPATH QuickJs CACHE STRING "" FORCE)
endif ()
