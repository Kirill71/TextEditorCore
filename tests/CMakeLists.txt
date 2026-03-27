set(TARGET_NAME TextEditorCoreTests)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.zip
)

if(WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} PRIVATE tests.cpp)

# test, lib, exe in one place
set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

target_link_libraries(${TARGET_NAME} PRIVATE
        TextEditorCoreLib
        GTest::gtest_main
)

if(ENABLE_COVERAGE)
    add_custom_target(coverage
            COMMAND ctest --output-on-failure
            COMMAND lcov --capture
            --rc geninfo_unexecuted_blocks=1
            --ignore-errors gcov
            --directory ${CMAKE_BINARY_DIR}
            --output-file coverage.info
            COMMAND lcov --remove coverage.info '/usr/*' '*/_deps/*'
            --ignore-errors unused
            --output-file coverage.info
            COMMAND genhtml coverage.info --output-directory coverage_report
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

include(GoogleTest)

# specify work dir directly in order to find test files.
gtest_discover_tests(${TARGET_NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(TEST_FILES
    test.txt
    file.txt
    empty.txt
)

foreach(file ${TEST_FILES})
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${file}
            $<TARGET_FILE_DIR:${TARGET_NAME}>
    )
endforeach()