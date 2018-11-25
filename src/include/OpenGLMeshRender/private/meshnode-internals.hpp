#include "../bindoperation.h"
#include "../meshnode.h"

#include <cassert>
#include <type_traits>
#include <typeinfo>
#include <utility>


// We only manage GLfloat and GLdoule type
struct GLTypeId
{
    static std::type_info const& glFloatId;
    static std::type_info const& glDoubleId;
};

template <typename T>
void checkTypeId( T const& type )
{
    assert( GLTypeId::glFloatId.hash_code() == type.hash_code()
            || GLTypeId::glDoubleId.hash_code() == type.hash_code() );
}

// Here we want to set the enumValue to GL_FLOAT or GL_DOUBLE depending on the
// data type
template <typename Type, typename Enum>
void assignCorrectEnum( Type const& type, Enum& enumValue )
{
    if ( GLTypeId::glFloatId.hash_code() == type.hash_code() )
    {
        enumValue = GL_FLOAT;
    }
    else if ( GLTypeId::glDoubleId.hash_code() == type.hash_code() )
    {
        enumValue = GL_DOUBLE;
    }
}



template <typename MeshT, require_colors<MeshT> = dummy::value>
void tryInitColorType( MeshT const&, GLenum& colorsType )
{
    using colorT = decltype( std::declval<MeshT>().getColorsPointer() );

    auto const& colorId = typeid( std::remove_pointer_t<colorT> );


    checkTypeId( colorId );
    assignCorrectEnum( colorId, colorsType );
}
template <typename MeshT, require_no_colors<MeshT> = dummy::value>
void tryInitColorType( MeshT const&, GLenum& )
{
}

template <typename MeshT, require_uv<MeshT> = dummy::value>
void tryInitUVType( MeshT const&, GLenum& uvType )
{
    using uvT = decltype( std::declval<MeshT>().getUVPointer() );

    auto const& uvId = typeid( std::remove_pointer_t<uvT> );


    checkTypeId( uvId );
    assignCorrectEnum( uvId, uvType );
}
template <typename MeshT, require_no_uv<MeshT> = dummy::value>
void tryInitUVType( MeshT const&, GLenum& )
{
}



template <typename MeshT>
MeshNode<MeshT>::MeshNode( MeshT& meshView )
    : mesh_{meshView}
{
    tryInitColorType( mesh_, colorsType_ );

    tryInitUVType( mesh_, uvType_ );


    // We should always having points
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
    if ( numIndexes_ == 0 )
    {
        generateBuffers_();
    }


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
        glDeleteVertexArrays( 1, &vao_ );
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
void MeshNode<MeshT>::generateBuffers_()
{
    glGenBuffers( 1, &vertexBuffer_ );
    glGenBuffers( 1, &indexBuffer_ );
    glGenBuffers( 1, &colorBuffer_ );
    glGenBuffers( 1, &uvBuffer_ );

    glGenVertexArrays( 1, &vao_ );
}
