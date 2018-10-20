#include "../meshnode.h"


template <typename MeshT>
MeshNode<MeshT>::MeshNode( MeshT& meshView )
    : mesh_{meshView}
{
}

template <typename MeshT>
void MeshNode<MeshT>::updateVertexBuffer( int const positionLocation, int const colorLocation )
{
    // TODO this should be dependant on if the mesh have some function or not

    auto numIndexes_ = mesh_.getNumIndex();
    auto numVertices = mesh_.getNumVertices();
    auto numColors   = mesh_.getNumColors();

    auto* indexesPointer  = mesh_.getIndexesPointer();
    auto* verticesPointer = mesh_.getVerticesPointer();
    auto* colorsPointer   = mesh_.getColorsPointer();

    regenerateBuffers_();

    glBindVertexArray( vao_ );

    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer_ );
    glBufferData( GL_ARRAY_BUFFER, sizeof( decltype( *verticesPointer ) ) * numVertices,
                  verticesPointer, GL_STATIC_DRAW );

    glVertexAttribPointer( positionLocation, 3, GL_FLOAT, GL_FALSE,
                           sizeof( decltype( *verticesPointer ) ), 0 );
    glEnableVertexAttribArray( positionLocation );

    glBindBuffer( GL_ARRAY_BUFFER, colorBuffer_ );
    glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * numColors, colorsPointer, GL_STATIC_DRAW );

    glVertexAttribPointer( colorLocation, 2, GL_FLOAT, GL_FALSE, sizeof( GLfloat ) * 2, 0 );
    glEnableVertexAttribArray( colorLocation );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer_ );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * numIndexes_, indexesPointer,
                  GL_STATIC_DRAW );

    glBindVertexArray( 0 );
}

template <typename MeshT>
void MeshNode<MeshT>::draw() const
{
    //
    glBindVertexArray( vao_ );
    glDrawElements( GL_TRIANGLES, numIndexes_, GL_UNSIGNED_INT, 0 );
    glBindVertexArray( 0 );
}

template <typename MeshT>
MeshNode<MeshT>::~MeshNode()
{
    cleanBuffers_();
}

template <typename MeshT>
void MeshNode<MeshT>::cleanBuffers_()
{
    if ( glIsBuffer( vao_ ) )
    {
        glDeleteBuffers( 1, &vao_ );
    }
    if ( glIsBuffer( indexBuffer_ ) )
    {
        glDeleteBuffers( 1, &indexBuffer_ );
    }
    if ( glIsBuffer( vertexBuffer_ ) )
    {
        glDeleteBuffers( 1, &vertexBuffer_ );
    }
    if ( glIsBuffer( colorBuffer_ ) )
    {
        glDeleteBuffers( 1, &colorBuffer_ );
    }
}

template <typename MeshT>
void MeshNode<MeshT>::regenerateBuffers_()
{
    cleanBuffers_();

    glGenBuffers( 1, &vertexBuffer_ );
    glGenBuffers( 1, &indexBuffer_ );
    glGenBuffers( 1, &colorBuffer_ );

    glGenVertexArrays( 1, &vao_ );
}
