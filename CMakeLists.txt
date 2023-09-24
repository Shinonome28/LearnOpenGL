cmake_minimum_required(VERSION 3.11)
project(LearnOpenGLCN)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(glm CONFIG REQUIRED)

add_subdirectory(src)

set(shaderSource "${CMAKE_SOURCE_DIR}/Shaders")
set(shaderDestination "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/Shaders")
add_custom_target(
    copyShader
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${shaderSource} ${shaderDestination} 
    DEPENDS ${shaderSource}
)

set(imageSource "${CMAKE_SOURCE_DIR}/images")
set(imageDestination "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/images")
add_custom_target(
    copyImages
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${imageSource} ${imageDestination} 
    DEPENDS ${imageSource}
)