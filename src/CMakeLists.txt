project(TextEditorCoreLib)

add_library(${PROJECT_NAME} SHARED)
target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(ENABLE_COVERAGE)
    target_compile_options(${PROJECT_NAME} PRIVATE -O0 -g --coverage)
    target_link_options(${PROJECT_NAME} PRIVATE --coverage)
endif()

target_sources(
        ${PROJECT_NAME}
        PRIVATE
            cursor.cpp
            finder.cpp
            replacer.cpp
            texteditorcore.cpp
            utils.cpp
            cursor.hpp
            finder.hpp
            customiterator.hpp
            replacer.hpp
            utils.hpp
        PUBLIC
        FILE_SET HEADERS FILES
            texteditorcore.hpp
            position.hpp
)