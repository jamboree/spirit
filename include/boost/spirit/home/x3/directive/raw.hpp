/*=============================================================================
    Copyright (c) 2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_X3_RAW_APRIL_9_2007_0912AM)
#define SPIRIT_X3_RAW_APRIL_9_2007_0912AM

#include <boost/spirit/home/x3/core/directive.hpp>
#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>
#include <boost/spirit/home/x3/support/traits/move_to.hpp>

namespace boost { namespace spirit { namespace x3
{
    // this is a pseudo attribute type indicating that the parser wants the
    // iterator range pointing to the [first, last) matching characters from
    // the input iterators.
    struct raw_attribute_type {};

    struct raw_directive : directive<raw_directive>
    {
        template <typename Subject>
        struct traits
        {
            typedef raw_attribute_type attribute_type;
            static bool const has_attribute = true;
        };

        template <typename Subject, typename Iterator, typename Context, typename Attribute>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            x3::skip_over(first, last, context);
            Iterator i(first);
            if (subject.parse(i, last, context, unused))
            {
                traits::move_to(first, i, attr);
                first = i;
                return true;
            }
            return false;
        }

        template <typename Subject, typename Iterator, typename Context>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, unused_type) const
        {
            return subject.parse(first, last, context, unused);
        }
    };

    raw_directive const raw{};
}}}

#endif
