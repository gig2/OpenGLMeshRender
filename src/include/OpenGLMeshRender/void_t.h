#pragma once

// This is what we will use intensivly with
// everything related to enable_if mechanism
// see https://www.fluentcpp.com/2018/05/15/make-sfinae-pretty-1-what-value-sfinae-brings-to-code/
// and the second part
// https://www.fluentcpp.com/2018/05/18/make-sfinae-pretty-2-hidden-beauty-sfinae/


template <typename...>
using tryToInstanciate = void;

struct dummy
{
    using type = short;
    static const type value{0};
};
