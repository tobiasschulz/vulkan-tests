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


set(SOURCE_COMMON
        src/vulkan/helper/io.cpp
        src/vulkan/helper/io.h
        3rdparty/backward/backward.cpp
        3rdparty/backward/backward.hpp
        src/vulkan/helper/helpers.cpp
        src/vulkan/helper/helpers.h
        src/vulkan/helper/SwapChainSupport.cpp
        src/vulkan/helper/SwapChainSupport.h
        src/vulkan/helper/QueueFamilies.cpp
        src/vulkan/helper/QueueFamilies.h
        src/vulkan/helper/Shader.cpp
        src/vulkan/helper/Shader.h
        src/vulkan/vulkan.cpp
        src/vulkan/vulkan.h
        src/vulkan/helper/Vertex.cpp
        src/vulkan/helper/Vertex.h
        src/vulkan/helper/Texture.cpp
        src/vulkan/helper/Texture.h
        src/vulkan/Mesh.cpp
        src/vulkan/Mesh.h
        src/vulkan/Renderer.cpp
        src/vulkan/Renderer.h
        src/vulkan/helper/TextureHelper.cpp
        src/vulkan/helper/TextureHelper.h
        src/vulkan/helper/BufferHelper.cpp
        src/vulkan/helper/BufferHelper.h
        src/vulkan/helper/UniformBuffer.cpp
        src/vulkan/helper/UniformBuffer.h
        src/vulkan/helper/UniformBufferObject.cpp
        src/vulkan/helper/UniformBufferObject.h
        src/vulkan/Camera.cpp
        src/vulkan/Camera.h
        src/vulkan/Buffer.cpp
        src/vulkan/Buffer.h
        )

# TEST 1
add_executable("vulkantest_1"
        src/test1/main.cpp
        src/test1/main.h
        ${SOURCE_COMMON})
target_link_libraries(vulkantest_1 ${COMMON_LINK_LIBS})

# TEST 2
add_executable("vulkantest_2"
        src/test2/main.cpp
        src/test2/main.h
        ${SOURCE_COMMON}
        )
target_link_libraries(vulkantest_2 ${COMMON_LINK_LIBS})

# TEST 3
add_executable("vulkantest_3"
        src/test3/main.cpp
        src/test3/main.h
        ${SOURCE_COMMON}
        )
target_link_libraries(vulkantest_3 ${COMMON_LINK_LIBS})
