cmake_minimum_required(VERSION 3.22)
project(PROJECT_NAME)

set(CMAKE_C_STANDARD 99)

# Adding our source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/scripts/*.c")
file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/scripts/*.h")
#set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/scripts/") # Define PROJECT_INCLUDE to be the path to the include directory of the project
#set(PROJECT_INCLUDE_DISPLAY "${CMAKE_CURRENT_LIST_DIR}/scripts/display/") # Define PROJECT_INCLUDE to be the path to the include directory of the project

# --- SDL2 SETUP ---
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(SDL2_PATH "SDL2/x86_64-w64-mingw32")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
        scripts/vector.c
        scripts/mesh.c
        scripts/mesh.h
        scripts/triangle.c
        scripts/triangle.h
        scripts/array.c
        scripts/array.h
        scripts/array.h)
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
#target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DISPLAY}  ${PROJECT_INCLUDE})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} )

target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
