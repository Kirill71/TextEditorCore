cmake_minimum_required(VERSION 3.28)
project(TextEditorCore VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTING "Enable tests" ON)
option(ENABLE_COVERAGE "Enable coverage reporting" ON)

if (MSVC)
    MESSAGE(STATUS "Test coverage is not supported on the Windows platform")
    set(ENABLE_COVERAGE OFF)
endif()

add_subdirectory(src)

if (BUILD_TESTING)
    message(STATUS "Testing is enabled")
    enable_testing()
    add_subdirectory(tests)
endif ()

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE TextEditorCoreLib)