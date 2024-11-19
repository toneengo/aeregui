cmake_minimum_required(VERSION 3.12)

if (WIN32)
    if( EXISTS $ENV{VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake )
        message(STATUS "Found $ENV{VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake; using it!")
        set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake
            CACHE STRING "Vcpkg toolchain file")
    endif()
endif(WIN32)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project("aeregui")

add_library(aeregui)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "aeregui")

find_package(OpenGL REQUIRED)
target_link_libraries(aeregui PRIVATE OpenGL::GL)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(aeregui PRIVATE glfw)

find_package(Freetype REQUIRED)
target_link_libraries(aeregui PRIVATE Freetype::Freetype)

find_package(glm CONFIG REQUIRED)
target_link_libraries(aeregui PRIVATE glm::glm)

add_subdirectory(src)
add_subdirectory(include/src)

target_include_directories(aeregui PRIVATE "include/")
target_include_directories(aeregui PRIVATE "resources/")
target_include_directories(aeregui PRIVATE "src/")

#add_dependencies(aeregui assets)
