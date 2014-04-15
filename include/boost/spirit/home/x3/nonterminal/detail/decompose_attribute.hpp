/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_DETAIL_ATTRIBUTE_HELPER_HPP_INCLUDED
#define BOOST_SPIRIT_X3_DETAIL_ATTRIBUTE_HELPER_HPP_INCLUDED


#include <tuple>


namespace boost { namespace spirit { namespace x3 { namespace detail
{
    template <typename Attribute>
    struct decompose_attribute
    {
        typedef Attribute result_type;
        typedef std::tuple<> params_type;
    };
    
    template <typename Attribute, typename... Ts>
    struct decompose_attribute<Attribute(Ts...)>
    {
        typedef Attribute result_type;
        typedef std::tuple<Ts...> params_type;
    };
}}}}


#endif

