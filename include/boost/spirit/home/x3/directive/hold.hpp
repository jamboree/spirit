/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_HOLD_FEBRUARY_6_2010_0917AM)
#define SPIRIT_HOLD_FEBRUARY_6_2010_0917AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/directive.hpp>
#include <boost/spirit/home/x3/support/traits/move_to.hpp>

namespace boost { namespace spirit { namespace x3
{
    struct hold_directive : directive<hold_directive>
    {
        static bool const is_pass_through_unary = true;
        
        template <typename Subject, typename Iterator, typename Context, typename Attribute>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            Attribute copy(attr);
            if (subject.parse(first, last, context, copy))
            {
                traits::move_to(copy, attr);
                return true;
            }
            return false;
        }
    };

    hold_directive const hold{};
}}}

#endif
