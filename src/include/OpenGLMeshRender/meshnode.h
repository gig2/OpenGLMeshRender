#pragma once

#include <GL/glew.h>

#include <cstddef>

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

    GLuint vao_{0};
    GLuint indexBuffer_{0};
    GLuint vertexBuffer_{0};
    GLuint colorBuffer_{0};

    std::size_t numIndexes_{0};

    const unsigned int verticesNumberPerFace_{3};
    const unsigned int numberColors_{3};

    void cleanBuffers_();

    void regenerateBuffers_();
};

#include "private/meshnode-internals.hpp"
