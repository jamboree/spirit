/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_OPAQUE_HPP_INCLUDED
#define BOOST_SPIRIT_X3_OPAQUE_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/caller.hpp>
#include <boost/utility/result_of.hpp>


namespace boost { namespace spirit { namespace x3
{
    template <typename Gen>
    struct opaque : parser<opaque<Gen>>, result_of<Gen const()>::type
    {
        typedef typename result_of<Gen const()>::type base_type;
        
        explicit opaque(Gen const& gen)
          : base_type(gen()) {}
    };
    
    template <typename Gen>
    inline opaque<Gen> make_opaque(Gen const& gen)
    {
        return opaque<Gen>(gen);
    }
}}}

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    template <typename Gen, typename Context>
    struct attribute_of<opaque<Gen>, Context>
      : attribute_of<typename opaque<Gen>::base_type, Context>
    {};
    
    template <typename Gen, typename Context>
    struct has_attribute<opaque<Gen>, Context>
      : has_attribute<typename opaque<Gen>::base_type, Context>
    {};
    
    template <typename Gen, typename Context>
    struct handles_container<opaque<Gen>, Context>
      : handles_container<typename opaque<Gen>::base_type, Context>
    {};
    
    // Caller
    //--------------------------------------------------------------------------
    template <typename Gen, typename... Ts, typename Context>
    struct attribute_of<caller<opaque<Gen>, Ts...>, Context>
      : attribute_of<caller<typename opaque<Gen>::base_type, Ts...>, Context>
    {};
    
    template <typename Gen, typename... Ts, typename Context>
    struct has_attribute<caller<opaque<Gen>, Ts...>, Context>
      : has_attribute<caller<typename opaque<Gen>::base_type, Ts...>, Context>
    {};
    
    template <typename Gen, typename... Ts, typename Context>
    struct handles_container<caller<opaque<Gen>, Ts...>, Context>
      : handles_container<caller<typename opaque<Gen>::base_type, Ts...>, Context>
    {};
}}}}


#endif

