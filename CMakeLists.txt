cmake_minimum_required(VERSION 4.1.2)
set(CMAKE_GENERATOR Ninja)
find_package(Doxygen REQUIRED dot)
project(generichelper C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g3")
set(DOXYGEN_PROJECT_NAME "Generic helper / demonstration library")
set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_RECURSIVE YES)
set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
set(DOXYGEN_GENERATE_HTMLHELP YES)
set(DOXYGEN_CALL_GRAPH YES)
set(DOXYGEN_GENERATE_LATEX YES)
set(DOXYGEN_USE_PDFLATEX YES)
set(DOXYGEN_PDF_HYPERLINKS YES)
set(DOXYGEN_ENABLE_PREPROCESSING NO)




doxygen_add_docs(docs ALL COMMENT "Generating Doxygen Documentation")
add_dependencies(docs generichelper)
add_library(generichelper SHARED
        decode_helpers.c
        decode_helpers.h
        decode_helpers.c
        linked_list.c
        linked_list.h
        accelerations.c
        accelerations.h)
add_subdirectory(tests)
