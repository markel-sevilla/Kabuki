cmake_minimum_required(VERSION 3.8)
project(k-lib)

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# Compiler options (for all targets)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
    add_compile_options(/W3)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else ()
    add_compile_options(-Wextra -pedantic -Wconversion)

    if(CMAKE_COMPILE_WARNING_AS_ERROR)
        add_compile_options(-Werror)
    endif()
endif ()

# For Clion/VSCode
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()
add_subdirectory(src)
add_subdirectory(test)

# Static analysis
include(dev-tools.cmake)
