cmake_minimum_required(VERSION 3.4)
project(vulkantest)

# Initialize cmake options
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CONFIGURATION_TYPES Debug) # This seems to be needed to actually change

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Setup 3rd party dependencies.
set(3RD_PARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/3rdparty)

find_package(OpenGL)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# need to compile GLFW and JSON
add_subdirectory("${3RD_PARTY_DIR}/glfw")
add_subdirectory("${3RD_PARTY_DIR}/glm")

include_directories(SYSTEM
        "${3RD_PARTY_DIR}/glfw/include"
        "${3RD_PARTY_DIR}/glm"
)

set(COMMON_LINK_LIBS ${CMAKE_DL_LIBS} glfw ${OPENGL_LIBRARIES})

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif (UNIX)

add_executable(vulkantest_1,
        test1.cpp
        common.cpp
        backward.cpp
)

target_link_libraries(vulkantest_1 ${COMMON_LINK_LIBS})