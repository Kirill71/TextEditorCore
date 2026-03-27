cmake_minimum_required(VERSION 3.28)
project(TextEditorCore VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTING "Enable tests" ON)
option(ENABLE_COVERAGE "Enable coverage reporting" ON)
set (TEST_WORK_DIR ${CMAKE_BINARY_DIR}/bin CACHE STRING "Working directory for tests" FORCE)

if (WIN32)
    MESSAGE(STATUS "Test coverage is not supported on the Windows platform")
    set(ENABLE_COVERAGE OFF)
endif()

add_subdirectory(src)

add_executable(${PROJECT_NAME})
# test, lib, exe in one place
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE TextEditorCoreLib)

if (BUILD_TESTING)
    message(STATUS "Testing is enabled")
    enable_testing()
    add_subdirectory(tests)
endif ()
