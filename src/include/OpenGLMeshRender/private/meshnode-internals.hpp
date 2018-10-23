#include "../meshnode.h"

#include <cassert>
#include <type_traits>
#include <typeinfo>
#include <utility>



template <typename MeshT>
MeshNode<MeshT>::MeshNode( MeshT& meshView )
    : mesh_{meshView}
{
    // We only manage GLfloat and GLdoule type
    auto const& glFloatId  = typeid( GLfloat );
    auto const& glDoubleId = typeid( GLdouble );


    // For this lamdba we do not need to capture glFloatId and glDoubleId since they are a
    // reference initialized with a constant expression

    // this will assert when the data type is not an OpenGL like
    auto checkTypeId = []( auto const& type ) {
        assert( glFloatId.hash_code() == type.hash_code()
                || glDoubleId.hash_code() == type.hash_code() );
    };

    // Here we want to set the enumValue to GL_FLOAT or GL_DOUBLE depending on the
    // data type
    auto assignCorrectEnum = []( auto const& type, auto& enumValue ) {
        if ( glFloatId.hash_code() == type.hash_code() )
        {
            enumValue = GL_FLOAT;
        }
        else if ( glDoubleId.hash_code() == type.hash_code() )
        {
            enumValue = GL_DOUBLE;
        }
    };


    using colorT = decltype( std::declval<MeshT>().getColorsPointer() );

    auto const& colorId = typeid( std::remove_pointer_t<colorT> );


    checkTypeId( colorId );
    assignCorrectEnum( colorId, colorsType );

    // for verticesT we want to get the data type of the element of a vertex:
    // ie the data type of the component
    using verticesT = typename MeshT::PointScalarT;

    auto const& verticesId = typeid( verticesT );

    checkTypeId( verticesId );
    assignCorrectEnum( verticesId, positionType );
}

template <typename MeshT>
void MeshNode<MeshT>::updateVertexBuffer( int const positionLocation, int const colorLocation )
{
    // TODO this should be dependant on if the mesh have some function or not

    numIndexes_      = mesh_.getNumIndex();
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

    glVertexAttribPointer( positionLocation, numberPointCoordinates_, positionType, GL_FALSE,
                           sizeof( decltype( *verticesPointer ) ), 0 );
    glEnableVertexAttribArray( positionLocation );

    glBindBuffer( GL_ARRAY_BUFFER, colorBuffer_ );
    glBufferData( GL_ARRAY_BUFFER, sizeof( *colorsPointer ) * numColors, colorsPointer,
                  GL_STATIC_DRAW );

    glVertexAttribPointer( colorLocation, numberColorsPerVertex_, colorsType, GL_FALSE,
                           sizeof( *colorsPointer ) * numberColorsPerVertex_, 0 );
    glEnableVertexAttribArray( colorLocation );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer_ );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( *indexesPointer ) * numIndexes_, indexesPointer,
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
