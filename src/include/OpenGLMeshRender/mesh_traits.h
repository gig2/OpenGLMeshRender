#include "void_t.h"

#include <type_traits>

// This is what we will use intensivly with
// everything related to enable_if mechanism
// see https://www.fluentcpp.com/2018/05/15/make-sfinae-pretty-1-what-value-sfinae-brings-to-code/
// and the second part
// https://www.fluentcpp.com/2018/05/18/make-sfinae-pretty-2-hidden-beauty-sfinae/



// So it is a multiple part process
// First we begin by declaring some traits at false_type

template <typename T, typename Enabled = void>
struct has_vertices : std::false_type
{
};

template <typename T, typename Enabled = void>
struct has_colors : std::false_type
{
};

template <typename T, typename Enabled = void>
struct has_uv : std::false_type
{
};

// then we specialize those to true_type for Mesh that can
// call the expected function

template <typename MeshT>
struct has_vertices<MeshT, tryToInstanciate<decltype( std::declval<MeshT>().getVerticesPointer() )>>
    : std::true_type
{
};

template <typename MeshT>
struct has_colors<MeshT, tryToInstanciate<decltype( std::declval<MeshT>().getColorsPointer() )>>
    : std::true_type
{
};

template <typename MeshT>
struct has_uv<MeshT, tryToInstanciate<decltype( std::declval<MeshT>().getUVPointer() )>>
    : std::true_type
{
};

// now we make SFINAE more expressive
// so that we can use them more readable
// in our template parameter

template <typename T>
using require_vertices = std::enable_if_t<has_vertices<T>::value, dummy::type>;

template <typename T>
using require_no_vertices = std::enable_if_t<!has_vertices<T>::value, dummy::type>;

template <typename T>
using require_colors = std::enable_if_t<has_colors<T>::value, dummy::type>;

template <typename T>
using require_no_colors = std::enable_if_t<!has_colors<T>::value, dummy::type>;

template <typename T>
using require_uv = std::enable_if_t<has_uv<T>::value, dummy::type>;

template <typename T>
using require_no_uv = std::enable_if_t<!has_uv<T>::value, dummy::type>;
