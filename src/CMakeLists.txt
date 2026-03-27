set(TARGET_NAME TextEditorCoreLib)

add_library(${TARGET_NAME} SHARED)

target_compile_definitions(${TARGET_NAME} PRIVATE TEXTEDITORCORELIB_EXPORTS)

# test, lib, exe in one place
set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(ENABLE_COVERAGE)
    target_compile_options(${TARGET_NAME} PRIVATE -O0 -g --coverage)
    target_link_options(${TARGET_NAME} PRIVATE --coverage)
endif()

target_sources(
        ${TARGET_NAME}
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
            export.hpp
        PUBLIC
        FILE_SET HEADERS FILES
            texteditorcore.hpp
            position.hpp
)