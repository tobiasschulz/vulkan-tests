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
find_package(Vulkan)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# need to compile GLFW and JSON
add_subdirectory("${3RD_PARTY_DIR}/glfw")
add_subdirectory("${3RD_PARTY_DIR}/glm")

include_directories(SYSTEM
        "${3RD_PARTY_DIR}/vulkan-hpp"
        "${3RD_PARTY_DIR}/glfw/include"
        "${3RD_PARTY_DIR}/glm"
        "${VULKAN_INCLUDE_DIR}"
        )

MESSAGE(STATUS "VULKAN_INCLUDE_DIR: " ${VULKAN_INCLUDE_DIR})


add_definitions(
        -DGLEW_STATIC
)

set(COMMON_LINK_LIBS ${CMAKE_DL_LIBS} ${OPENGL_LIBRARIES} glfw ${VULKAN_LIBRARY})

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
    set(COMMON_LINK_LIBS ${COMMON_LINK_LIBS} bfd)
endif (UNIX)

# TEST 1
add_executable("vulkantest_1"
        src/test1/main.cpp
        src/test1/main.h
        src/helper/io.cpp
        src/helper/io.h
        3rdparty/backward/backward.cpp
        3rdparty/backward/backward.hpp
        src/helper/VulkanHelper.cpp
        src/helper/VulkanHelper.h
        src/helper/SwapChainSupport.cpp
        src/helper/SwapChainSupport.h
        src/helper/QueueFamilies.cpp
        src/helper/QueueFamilies.h
        src/helper/Shader.cpp
        src/helper/Shader.h
        src/helper/vulkan.cpp
        src/helper/vulkan.h
        )
target_link_libraries(vulkantest_1 ${COMMON_LINK_LIBS})

# TEST 2
add_executable("vulkantest_2"
        src/test2/main.cpp
        src/test2/main.h
        src/helper/io.cpp
        src/helper/io.h
        3rdparty/backward/backward.cpp
        3rdparty/backward/backward.hpp
        src/helper/VulkanHelper.cpp
        src/helper/VulkanHelper.h
        src/helper/SwapChainSupport.cpp
        src/helper/SwapChainSupport.h
        src/helper/QueueFamilies.cpp
        src/helper/QueueFamilies.h
        src/helper/Shader.cpp
        src/helper/Shader.h
        src/helper/vulkan.cpp
        src/helper/vulkan.h
)
target_link_libraries(vulkantest_2 ${COMMON_LINK_LIBS})
