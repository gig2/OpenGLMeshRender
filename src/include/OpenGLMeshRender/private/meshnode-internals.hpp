#include "../bindoperation.h"
#include "../meshnode.h"

#include <cassert>
#include <type_traits>
#include <typeinfo>
#include <utility>



template <typename MeshT>
MeshNode<MeshT>::MeshNode( MeshT& meshView )
    : mesh_{meshView}
{
    // TODO:
    // Those operation should be done if the required traits is validated
    // and it then add those for uv
    // note that with c++17 we could do simple if constexpr with the required traits by the way
    // so we may want to check the minimum compiler version "allowed"

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
    assignCorrectEnum( colorId, colorsType_ );

    // for verticesT we want to get the data type of the element of a vertex:
    // ie the data type of the component
    using verticesT = typename MeshT::PointScalarT;

    auto const& verticesId = typeid( verticesT );

    checkTypeId( verticesId );
    assignCorrectEnum( verticesId, positionType_ );
}

template <typename MeshT>
void MeshNode<MeshT>::updateVertexBuffer( int const positionLocation, int const colorLocation,
                                          int const uvLocation )
{
    regenerateBuffers_();


    glBindVertexArray( vao_ );

    verticesBindBuffer( mesh_, positionLocation, vertexBuffer_, positionType_ );

    colorsBindBuffer( mesh_, colorLocation, colorBuffer_, colorsType_ );

    uvBindBuffer( mesh_, uvLocation, uvBuffer_, uvType_ );

    // For now we say that index is a must have
    // actually vertex position is needed at least

    // index

    numIndexes_          = mesh_.getNumIndex();
    auto* indexesPointer = mesh_.getIndexesPointer();

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
    if ( glIsBuffer( uvBuffer_ ) )
    {
        glDeleteBuffers( 1, &uvBuffer_ );
    }
}

template <typename MeshT>
void MeshNode<MeshT>::regenerateBuffers_()
{
    cleanBuffers_();

    glGenBuffers( 1, &vertexBuffer_ );
    glGenBuffers( 1, &indexBuffer_ );
    glGenBuffers( 1, &colorBuffer_ );
    glGenBuffers( 1, &uvBuffer_ );

    glGenVertexArrays( 1, &vao_ );
}
