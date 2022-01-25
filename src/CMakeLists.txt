cmake_minimum_required(VERSION 3.21)

execute_process(COMMAND git describe --tags --abbrev=0        RESULT_VARIABLE VerResult OUTPUT_VARIABLE VerString ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND git describe --tags --abbrev=7 --long RESULT_VARIABLE RevResult OUTPUT_VARIABLE RevString ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

set(SysString graphene)

if(${VerResult})
    set(VerString 0.0.0)
endif()

if(${RevResult})
    set(RevString 0.0.0-0-g0000000)
endif()

project(${SysString} VERSION ${VerString})
set(PROJECT_BRIEF "Cross platform 2d game engine")

set(PROJECT_RESOURCES
)

if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(OBJCOPY_FLAGS -I binary -O pe-i386      -B i386)
    endif()
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(OBJCOPY_FLAGS -I binary -O pe-x86-64    -B i386)
    endif()
else(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(OBJCOPY_FLAGS -I binary -O elf32-i386   -B i386)
    endif()
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(OBJCOPY_FLAGS -I binary -O elf64-x86-64 -B i386)
    endif()
endif(WIN32)

set(PROJECT_OBJECTS "")
set(WORKPATH ${PROJECT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir)

foreach(RESOURCE ${PROJECT_RESOURCES})
    add_custom_command(OUTPUT ${WORKPATH}/${RESOURCE}.o WORKING_DIRECTRY ${PROJECT_SOURCE_DIR}
        COMMAND ${CMAKE_OBJCOPY} ${OBJCOPY_FLAGS} ${RESOURCE} ${WORKPATH}/${RESOURCE}.o
    )
    list(APPEND PROJECT_OBJECTS ${WORKPATH}/${RESOURCE}.o)
endforeach(RESOURCE)

configure_file(version.hpp.in ${PROJECT_SOURCE_DIR}/../include/${PROJECT_NAME}/version.hpp)
configure_file(../Doxyfile.in ${PROJECT_SOURCE_DIR}/../Doxyfile)

# add_executable(${PROJECT_NAME})
add_library(${PROJECT_NAME} STATIC)
# add_library(${PROJECT_NAME} SHARED)

set_target_properties(${PROJECT_NAME}
PROPERTIES
    CXX_EXTENSIONS NO
    LINK_FLAGS_RELEASE -s -static
    IMPORTED_OBJECTS ${PROJECT_OBJECTS}
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../lib
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../lib
)

target_include_directories(${PROJECT_NAME}
PRIVATE
    ${PROJECT_SOURCE_DIR}/../include
)

target_link_directories(${PROJECT_NAME}
PRIVATE
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
)

target_link_options(${PROJECT_NAME}
PRIVATE
)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
)

target_compile_features(${PROJECT_NAME}
PRIVATE
    c_std_17
    cxx_std_20
)

target_compile_options(${PROJECT_NAME}
PRIVATE
    -Wall
    -pedantic-errors
)

target_sources(${PROJECT_NAME}
PRIVATE
)
