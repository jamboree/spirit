/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c)      2010 Bryce Lelbach
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_AS_DECEMBER_6_2010_1013AM)
#define SPIRIT_AS_DECEMBER_6_2010_1013AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/directive.hpp>
#include <string>

namespace boost { namespace spirit { namespace x3
{
    template <typename T>
    struct as : directive<as<T>>
    {
        template <typename Subject>
        struct traits
        {
            typedef T attribute_type;
            static bool const has_attribute = true;
        };
        
        template <typename Subject, typename Iterator, typename Context, typename Attribute>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            T as_attr;
            if (subject.parse(first, last, context, as_attr))
            {
                traits::move_to(as_attr, attr);
                return true;
            }
            return false;
        }
        
        template <typename Subject, typename Iterator, typename Context>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, T& attr) const
        {
            return subject.parse(first, last, context, attr);
        }
    };
    
    as<std::string> const as_string{};
}}}

#endif
