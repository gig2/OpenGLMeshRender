cmake_minimum_required(VERSION 3.5)
project(OpenGLMeshRender VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(GLEW REQUIRED)

include(GNUInstallDirs)

add_subdirectory(src)


install(EXPORT OpenGLMeshRenderConfig DESTINATION share/OpenGLMeshRender/cmake)

export(TARGETS OpenGLMeshRender FILE OpenGLMeshRenderConfig.cmake)
