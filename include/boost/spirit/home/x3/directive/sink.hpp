/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_SINK_HPP_INCLUDED
#define BOOST_SPIRIT_X3_SINK_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/directive.hpp>

namespace boost { namespace spirit { namespace x3
{
    struct sink_directive : directive<sink_directive>
    {
        template <typename Subject>
        struct traits
        {
            typedef unused_type attribute_type;
            static bool const has_attribute = false;
        };
        
        template <typename Subject, typename Iterator, typename Context, typename Attribute, typename T>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& /*attr*/, T& sink) const
        {
            return subject.parse(first, last, context, sink);
        }
    };
    
    sink_directive const sink{};
}}}

#endif
