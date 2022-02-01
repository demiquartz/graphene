cmake_minimum_required(VERSION 3.13)

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
else()
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(OBJCOPY_FLAGS -I binary -O elf32-i386   -B i386)
    endif()
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(OBJCOPY_FLAGS -I binary -O elf64-x86-64 -B i386)
    endif()
endif()

set(PROJECT_OBJECTS "")
set(WORKPATH ${PROJECT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir)
foreach(RESOURCE ${PROJECT_RESOURCES})
    set(OBJECT ${WORKPATH}/${RESOURCE}${CMAKE_C_OUTPUT_EXTENSION})
    add_custom_command(OUTPUT ${OBJECT}
        COMMAND ${CMAKE_OBJCOPY} ${OBJCOPY_FLAGS} ${RESOURCE} ${OBJECT}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} VERBATIM
    )
    list(APPEND PROJECT_OBJECTS ${OBJECT})
endforeach()

option(USE_GLFW "Use GLFW as a component" ON)
option(USE_DXGI "Use DXGI as a component" OFF)

find_package(X11 QUIET)
set(WINDOW_SYSTEM_WIN32 ${WIN32})
set(WINDOW_SYSTEM_X11 ${X11_FOUND})

configure_file(config.hpp.in  ${PROJECT_SOURCE_DIR}/config.hpp )
configure_file(version.hpp.in ${PROJECT_SOURCE_DIR}/version.hpp)
configure_file(../Doxyfile.in ${PROJECT_SOURCE_DIR}/../Doxyfile)

add_library(${PROJECT_NAME} ${PROJECT_OBJECTS})

set_target_properties(${PROJECT_NAME}
PROPERTIES
    CXX_EXTENSIONS NO
    LINK_FLAGS_RELEASE -s
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
    c_std_11
    cxx_std_20
)

target_compile_options(${PROJECT_NAME}
PRIVATE
    -Wall
    -pedantic-errors
)

target_sources(${PROJECT_NAME}
PRIVATE
    graphics/screen.cpp
    graphics/window.cpp
    graphene.cpp
)

if(USE_GLFW)
    if(BUILD_SHARED_LIBS)
        find_package(glfw3 3.3 REQUIRED)
        target_link_libraries(${PROJECT_NAME}
        PRIVATE
            glfw
        )
    endif()
    target_sources(${PROJECT_NAME}
    PRIVATE
        graphics/window/glfw.cpp
        setup/glfw.cpp
    )
endif()

if(USE_DXGI)
    target_sources(${PROJECT_NAME}
    PRIVATE
        graphics/screen/dxgi.cpp
    )
endif()
