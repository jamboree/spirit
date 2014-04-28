/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_REFERENCE_HPP_INCLUDED
#define BOOST_SPIRIT_X3_REFERENCE_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/detail/transform_params.hpp>
#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/support/traits/has_attribute.hpp>
#include <boost/spirit/home/x3/support/traits/handles_container.hpp>


namespace boost { namespace spirit { namespace x3
{
    template <typename Subject>
    struct reference : parser<reference<Subject>>
    {
        reference(Subject& ref)
          : ref(ref) {}
        
        template <typename... Ts>
        typename detail::transform_params<Subject, void, Ts...>::sfinae_result
        transform_params(Ts&&... ts) const
        {
            return ref.transform_params(std::forward<Ts>(ts)...);
        }

        template <typename Iterator, typename Context, typename Attribute, typename... Ts>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, Ts&&... ts) const
        {
            return ref.parse(first, last, context, attr, std::forward<Ts>(ts)...);
        }
        
        Subject& ref;
    };
}}}

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    template <typename Subject, typename Context>
    struct attribute_of<reference<Subject>, Context>
      : attribute_of<Subject, Context>
    {};
    
    template <typename Subject, typename Context>
    struct has_attribute<reference<Subject>, Context>
      : has_attribute<Subject, Context>
    {};
    
    template <typename Subject, typename Context>
    struct handles_container<reference<Subject>, Context>
      : handles_container<Subject, Context>
    {};
    
    // Caller
    //--------------------------------------------------------------------------
    template <typename Subject, typename... Ts, typename Context>
    struct attribute_of<caller<reference<Subject>, Ts...>, Context>
      : attribute_of<caller<Subject, Ts...>, Context>
    {};
    
    template <typename Subject, typename... Ts, typename Context>
    struct has_attribute<caller<reference<Subject>, Ts...>, Context>
      : has_attribute<caller<Subject, Ts...>, Context>
    {};
    
    template <typename Subject, typename... Ts, typename Context>
    struct handles_container<caller<reference<Subject>, Ts...>, Context>
      : handles_container<caller<Subject, Ts...>, Context>
    {};
}}}}


#endif

