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

    void updateVertexBuffer( int const positionLocation = 0, int const colorLocation = 1,
                             int const uvLocation = 2 );

    void draw() const;

    ~MeshNode();

    bool needToUpdate{true};

private:
    MeshT &mesh_;

    GLenum positionType_{GL_FLOAT};
    GLenum colorsType_{GL_FLOAT};
    GLenum uvType_{GL_FLOAT};

    GLuint vao_{0};

    GLuint indexBuffer_{0};
    GLuint vertexBuffer_{0};
    GLuint colorBuffer_{0};
    GLuint uvBuffer_{0};

    using indexT = decltype( std::declval<MeshT>().getNumIndex() );

    indexT numIndexes_{0};

    unsigned int const numberPointCoordinates_{3};
    unsigned int const verticesNumberPerFace_{3};
    unsigned int const numberColorsPerVertex_{3};
    unsigned int const numberUvPerVertex_{2};

    void cleanBuffers_();

    void regenerateBuffers_();
};

#include "private/meshnode-internals.hpp"
