#include "mesh_traits.h"

#include <GL/glew.h>

// Here we do some tag dispatching using SFINAE

template <typename MeshT, require_vertices<MeshT> = dummy::value>
void verticesBindBuffer( MeshT const &mesh, int const location, GLuint buffer, GLenum dataType )
{
    unsigned int const numberPointCoordinates = 3;


    auto numVertices      = mesh.getNumVertices();
    auto *verticesPointer = mesh.getVerticesPointer();

    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( decltype( *verticesPointer ) ) * numVertices,
                  verticesPointer, GL_STATIC_DRAW );

    glVertexAttribPointer( location, numberPointCoordinates, dataType, GL_FALSE,
                           sizeof( decltype( *verticesPointer ) ), 0 );
    glEnableVertexAttribArray( location );
}

template <typename MeshT, require_no_vertices<MeshT> = dummy::value>
void verticesBindBuffer( MeshT const &, int const, GLuint, GLenum )
{
}

template <typename MeshT, require_colors<MeshT> = dummy::value>
void colorsBindBuffer( MeshT const &mesh, int const location, GLuint buffer, GLenum dataType )
{
    // colors at vertex

    int const numberColorsPerVertex{3};

    auto numColors      = mesh.getNumColors();
    auto *colorsPointer = mesh.getColorsPointer();

    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( *colorsPointer ) * numColors, colorsPointer,
                  GL_STATIC_DRAW );

    glVertexAttribPointer( location, numberColorsPerVertex, dataType, GL_FALSE,
                           sizeof( *colorsPointer ) * numberColorsPerVertex, 0 );
    glEnableVertexAttribArray( location );
}

template <typename MeshT, require_no_colors<MeshT> = dummy::value>
void colorsBindBuffer( MeshT const &, int const, GLuint, GLenum )
{
}

template <typename MeshT, require_uv<MeshT> = dummy::value>
void uvBindBuffer( MeshT const &mesh, int const location, GLuint buffer, GLenum dataType )
{
    // UV at vertex

    int const numberUvPerVertex{2};

    auto numUVs     = mesh.getNumUV();
    auto *uvPointer = mesh.getUVPointer();

    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( *uvPointer ) * numUVs, uvPointer, GL_STATIC_DRAW );

    glVertexAttribPointer( location, numberUvPerVertex, dataType, GL_FALSE,
                           sizeof( *uvPointer ) * numberUvPerVertex, 0 );
    glEnableVertexAttribArray( location );
}

template <typename MeshT, require_no_uv<MeshT> = dummy::value>
void uvBindBuffer( MeshT const &, int const, GLuint, GLenum )
{
}
