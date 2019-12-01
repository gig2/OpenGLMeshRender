project('OpenGLMeshRender','cpp', default_options : ['cpp_std=c++17'],
       version: '0.0.0')

epoxy_dep = dependency('epoxy', required:true)
subdir('src')

# pkgconfig section
pkg = import('pkgconfig')

pkg.generate(description: 'OpenGLMeshRender is a simple utility to render mesh',
             filebase: 'OpenGLMeshRender',
             libraries: [openglmeshrender_lib],
             name: 'OpenGLMeshRender',
             subdirs: 'OpenGLMeshRender',
             version: meson.project_version()
            )
