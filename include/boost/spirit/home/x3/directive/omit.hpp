/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_OMIT_MARCH_24_2007_0802AM)
#define SPIRIT_OMIT_MARCH_24_2007_0802AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/support/unused.hpp>
#include <boost/spirit/home/x3/core/directive.hpp>

namespace boost { namespace spirit { namespace x3
{
    ///////////////////////////////////////////////////////////////////////////
    // omit_directive forces the attribute of subject parser
    // to be unused_type
    ///////////////////////////////////////////////////////////////////////////
    
    struct omit_directive : directive<omit_directive>
    {
        template <typename Subject>
        struct traits
        {
            typedef unused_type attribute_type;
            static bool const has_attribute = false;
        };

        template <typename Subject, typename Iterator, typename Context>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, unused_type) const
        {
            return subject.parse(first, last, context, unused);
        }
    };

    omit_directive const omit{};
}}}

#endif
