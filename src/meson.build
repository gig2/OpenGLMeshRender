openglmeshrender_headers = files([
                                  'include/OpenGLMeshRender/meshnode.h',
                                  'include/OpenGLMeshRender/private/meshnode-internals.hpp',])

openglmeshrender_include = include_directories('./include')


install_subdir('include/OpenGLMeshRender', install_dir:'include')


openglmeshrender_lib = library('OpenGLMeshRender',
                           sources: [openglmeshrender_headers],
                           dependencies: [glew_dep],
                           include_directories: [openglmeshrender_include],
                              install:true)

openglmeshrender_dep = declare_dependency(link_with: openglmeshrender_lib,
                                       dependencies: [glew_dep],
                                       include_directories: [openglmeshrender_include]
                                         )
