project(TextEditorCoreTests)
add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE tests.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
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
gtest_discover_tests(${PROJECT_NAME})

set(TEST_FILES
    test.txt
    file.txt
    empty.txt
)

foreach(file ${TEST_FILES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${file}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endforeach()