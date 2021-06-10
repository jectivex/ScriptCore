if (NOT EXISTS "${SCRIPTCORE_TEST_LIBS}")

    message(STATUS "SCRIPTCORE_TEST_LIBS ${SCRIPTCORE_TEST_LIBS} is empty. "
            "Downloading test libs including V8, JavaScriptCore, Lua, etc.")

    configure_file(${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt.in ScriptCoreLibsDownload/CMakeLists.txt)

    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ScriptCoreLibsDownload)

    if (result)
        message(FATAL_ERROR "CMake step for download ScriptCoreLibs failed: ${result}")
    endif ()

    execute_process(COMMAND ${CMAKE_COMMAND} --build .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ScriptCoreLibsDownload)

    if (result)
        message(FATAL_ERROR "Build step for download ScriptCoreLibs failed: ${result}")
    endif ()
elseif (("${SCRIPTCORE_TEST_FORCE_UPDATE_DEPS}" STREQUAL ON) OR ("$ENV{SCRIPTCORE_TEST_FORCE_UPDATE_DEPS}" STREQUAL ON))
    message(STATUS "update SCRIPTCORE_TEST_LIBS ${SCRIPTCORE_TEST_LIBS}")
    execute_process(COMMAND git pull --force --rebase WORKING_DIRECTORY ${SCRIPTCORE_TEST_LIBS})
endif ()
