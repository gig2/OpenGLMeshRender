set (openglmeshrender_headers
     include/OpenGLMeshRender/meshnode.h
     include/OpenGLMeshRender/private/meshnode-internals.hpp
     include/OpenGLMeshRender/bindoperation.h
     include/OpenGLMeshRender/mesh_traits.h
     include/OpenGLMeshRender/void_t.h)

set (openglmeshrender_sources
     meshnode.cpp)

add_library(OpenGLMeshRender ${openglmeshrender_headers} ${openglmeshrender_sources})

target_link_libraries(OpenGLMeshRender PUBLIC
  GLEW::GLEW)

target_include_directories(OpenGLMeshRender PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

install(TARGETS OpenGLMeshRender EXPORT OpenGLMeshRenderConfig
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR},
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

set_target_properties(OpenGLMeshRender PROPERTIES LINKER_LANGUAGE CXX)


install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
