#pragma once

#include <GL/glew.h>

#include <cstddef>
#include <typeinfo>
#include <utility>

template <typename MeshT>
class MeshNode
{
public:
    explicit MeshNode( MeshT &meshView );

    void updateVertexBuffer( int const positionLocation, int const colorLocation );

    void draw() const;

    ~MeshNode();

    bool needToUpdate{true};

private:
    MeshT &mesh_;

    GLenum positionType{GL_FLOAT};
    GLenum colorsType{GL_FLOAT};

    GLuint vao_{0};
    GLuint indexBuffer_{0};
    GLuint vertexBuffer_{0};
    GLuint colorBuffer_{0};

    decltype( std::declval<MeshT>().getNumIndex() ) numIndexes_{0};

    const unsigned int numberPointCoordinates_{3};
    const unsigned int verticesNumberPerFace_{3};
    const unsigned int numberColorsPerVertex_{3};

    void cleanBuffers_();

    void regenerateBuffers_();
};

#include "private/meshnode-internals.hpp"
