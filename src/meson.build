openglmeshrender_headers = files([
                                  'include/OpenGLMeshRender/meshnode.h',
                                  'include/OpenGLMeshRender/private/meshnode-internals.hpp',
                                  'include/OpenGLMeshRender/bindoperation.h',
                                  'include/OpenGLMeshRender/mesh_traits.h',
                                  'include/OpenGLMeshRender/void_t.h'])

openglmeshrender_sources = files(['meshnode.cpp'])

openglmeshrender_include = include_directories('./include')


install_subdir('include/OpenGLMeshRender', install_dir:'include')


openglmeshrender_lib = library('OpenGLMeshRender',
                               sources: [openglmeshrender_headers,
                                        openglmeshrender_sources],
                           dependencies: [epoxy_dep],
                           include_directories: [openglmeshrender_include],
                              install:true)

openglmeshrender_dep = declare_dependency(link_with: openglmeshrender_lib,
                                       dependencies: [epoxy_dep],
                                       include_directories: [openglmeshrender_include]
                                         )
