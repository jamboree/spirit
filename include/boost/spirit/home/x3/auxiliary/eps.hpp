/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_EPS_MARCH_23_2007_0454PM)
#define BOOST_SPIRIT_X3_EPS_MARCH_23_2007_0454PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>

namespace boost { namespace spirit { namespace x3
{
    struct eps_parser : parser<eps_parser>
    {
        typedef unused_type attribute_type;
        static bool const has_attribute = false;
        static bool const caller_is_pass_through_unary = true;

        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& /*attr*/, bool pred = true) const
        {
            x3::skip_over(first, last, context);
            return pred;
        }
    };

    eps_parser const eps{};
}}}

#endif
