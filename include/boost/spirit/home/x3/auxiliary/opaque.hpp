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
#include <boost/spirit/home/x3/core/detail/transform_params.hpp>
#include <boost/utility/result_of.hpp>


namespace boost { namespace spirit { namespace x3
{
    template <typename Gen>
    struct opaque
      : unary_parser<typename result_of<Gen const()>::type, opaque<Gen>>
    {
        typedef typename result_of<Gen const()>::type subject_type;
        typedef unary_parser<subject_type, opaque<Gen>> base_type;
        static bool const is_pass_through_unary = true;

        explicit opaque(Gen const& gen)
          : base_type(gen()) {}
        
        template <typename... Ts>
        typename detail::transform_params<subject_type, void, Ts...>::sfinae_result
        transform_params(Ts&&... ts) const
        {
            return this->subject.transform_params(std::forward<Ts>(ts)...);
        }
        
        template <typename Iterator, typename Context, typename Attribute, typename... Ts>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, Ts&&... ts) const
        {
            return this->subject.parse(
                first, last, context, attr, std::forward<Ts>(ts)...);
        }
    };
    
    template <typename Gen>
    inline opaque<Gen> make_opaque(Gen const& gen)
    {
        return opaque<Gen>(gen);
    }
}}}

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    template <typename Gen, typename... Ts, typename Context>
    struct attribute_of<caller<opaque<Gen>, Ts...>, Context>
      : attribute_of<caller<typename opaque<Gen>::subject_type, Ts...>, Context>
    {};
    
    template <typename Gen, typename... Ts, typename Context>
    struct has_attribute<caller<opaque<Gen>, Ts...>, Context>
      : has_attribute<caller<typename opaque<Gen>::subject_type, Ts...>, Context>
    {};
    
    template <typename Gen, typename... Ts, typename Context>
    struct handles_container<caller<opaque<Gen>, Ts...>, Context>
      : handles_container<caller<typename opaque<Gen>::subject_type, Ts...>, Context>
    {};
}}}}


#endif

